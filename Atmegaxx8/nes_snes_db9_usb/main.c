/* Nes/Snes/Genesis/SMS/Atari to USB
 * Copyright (C) 2006-2011 Rapha�l Ass�nat
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * The author may be contacted at raph@raphnet.net
 */
#include <avr/io.h>
//#include <avr/iom328p.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <string.h>

#include "usbdrv/usbdrv.h"
#include "usbdrv/oddebug.h"
#include "gamepad.h"

#include "snes.h"
//#include "snesmouse.h"
//#include "nes.h"
//#include "db9.h"
//#include "tg16.h"
//#include "segamtap.h"

#include "leds.h"
#include "devdesc.h"

static uchar *rt_usbHidReportDescriptor=NULL;
static uchar rt_usbHidReportDescriptorSize=0;
static uchar *rt_usbDeviceDescriptor=NULL;
static uchar rt_usbDeviceDescriptorSize=0;

#define MAX_REPORTS	8

PROGMEM const int usbDescriptorStringSerialNumber[]  = {
 	USB_STRING_DESCRIPTOR_HEADER(4),
	'1','0','0','0'
};

int usbDescriptorStringDevice[] = {
 	USB_STRING_DESCRIPTOR_HEADER(DEVICE_STRING_LENGTH),
	DEFAULT_PROD_STRING
};

char usbDescriptorConfiguration[] = { 0 }; // dummy

uchar my_usbDescriptorConfiguration[] = {    /* USB configuration descriptor */
    9,          /* sizeof(usbDescriptorConfiguration): length of descriptor in bytes */
    USBDESCR_CONFIG,    /* descriptor type */
    18 + 7 * USB_CFG_HAVE_INTRIN_ENDPOINT + 9, 0,
                /* total length of data returned (including inlined descriptors) */
    1,          /* number of interfaces in this configuration */
    1,          /* index of this configuration */
    0,          /* configuration name string index */
#if USB_CFG_IS_SELF_POWERED
    USBATTR_SELFPOWER,  /* attributes */
#else
    USBATTR_BUSPOWER,   /* attributes */
#endif
    USB_CFG_MAX_BUS_POWER/2,            /* max USB current in 2mA units */
/* interface descriptor follows inline: */
    9,          /* sizeof(usbDescrInterface): length of descriptor in bytes */
    USBDESCR_INTERFACE, /* descriptor type */
    0,          /* index of this interface */
    0,          /* alternate setting for this interface */
    USB_CFG_HAVE_INTRIN_ENDPOINT,   /* endpoints excl 0: number of endpoint descriptors to follow */
    USB_CFG_INTERFACE_CLASS,
    USB_CFG_INTERFACE_SUBCLASS,
    USB_CFG_INTERFACE_PROTOCOL,
    0,          /* string index for interface */
//#if (USB_CFG_DESCR_PROPS_HID & 0xff)    /* HID descriptor */
    9,          /* sizeof(usbDescrHID): length of descriptor in bytes */
    USBDESCR_HID,   /* descriptor type: HID */
    0x01, 0x01, /* BCD representation of HID version */
    0x00,       /* target country code */
    0x01,       /* number of HID Report (or other HID class) Descriptor infos to follow */
    0x22,       /* descriptor type: report */
    USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH, 0,  /* total length of report descriptor */
//#endif
#if USB_CFG_HAVE_INTRIN_ENDPOINT    /* endpoint descriptor for endpoint 1 */
    7,          /* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,  /* descriptor type = endpoint */
    0x81,       /* IN endpoint number 1 */
    0x03,       /* attrib: Interrupt endpoint */
    8, 0,       /* maximum packet size */
    USB_CFG_INTR_POLL_INTERVAL, /* in ms */
#endif
};

static Gamepad *curGamepad;


/* ----------------------- hardware I/O abstraction ------------------------ */

static void hardwareInit(void)
{
	uchar	i, j;

	// init port C as input with pullup EXCEPT C0-2 that are LED outputs
	DDRC = 0x05;
	PORTC = ~0x05;
	
	/* 1101 1000 bin: activate pull-ups except on USB lines 
	 *
	 * USB signals are on bit 0 and 2. 
	 *
	 * Bit 1 is connected with bit 0 (rev.C pcb error), so the pullup
	 * is not enabled.
	 * */
	PORTD = 0xf8;   

	/* Usb pin are init as outputs */
	DDRD = 0x01 | 0x04;    

	
	j = 0;
	while(--j){     /* USB Reset by device only required on Watchdog Reset */
		i = 0;
		while(--i); /* delay >10ms for USB reset */
	}
	DDRD = 0x00;    /* 0000 0000 bin: remove USB reset condition */
			/* configure timer 0 for a rate of 12M/(1024 * 256) = 45.78 Hz (~22ms) */


#if defined(__AVR_ATmega8__)
	//If using Atmega8 the registers are different so need tweaking
	TCCR0 = 5;      /* timer 0 prescaler: 1024 */

	TCCR2 = (1<<WGM21)|(1<<CS22)|(1<<CS21)|(1<<CS20);
	OCR2 = 196; // for 60 hz
#else
	//Default code for ATMEGA328 etc.
	TCCR0B = 5;      /* timer 0 prescaler: 1024 */

	//TODO CHECK me 20230920 then WGM21 is NOT in TCCR2B therefore it SHOULD not trigger on overflow of OCR2B value but happens to work?
	TCCR2B = (1<<WGM21)|(1<<CS22)|(1<<CS21)|(1<<CS20);
	OCR2B = 196; // for 60 hz
#endif
}

static uchar    reportBuffer[6];    /* buffer for HID reports */



/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

static uchar    idleRates[MAX_REPORTS];           /* in 4 ms units */

uchar	usbFunctionDescriptor(struct usbRequest *rq)
{
	if ((rq->bmRequestType & USBRQ_TYPE_MASK) != USBRQ_TYPE_STANDARD)
		return 0;

	if (rq->bRequest == USBRQ_GET_DESCRIPTOR)
	{
		// USB spec 9.4.3, high byte is descriptor type
		switch (rq->wValue.bytes[1])
		{
			case USBDESCR_DEVICE:
				usbMsgPtr = rt_usbDeviceDescriptor;		
				return rt_usbDeviceDescriptorSize;
			case USBDESCR_HID_REPORT:
				usbMsgPtr = rt_usbHidReportDescriptor;
				return rt_usbHidReportDescriptorSize;
			case USBDESCR_CONFIG:
				usbMsgPtr = my_usbDescriptorConfiguration;
				return sizeof(my_usbDescriptorConfiguration);
		}
	}

	return 0;
}

static uchar setupBuffer[sizeof(reportBuffer)];

uchar	usbFunctionSetup(uchar data[8])
{
	usbRequest_t    *rq = (void *)data;
	int i;

	usbMsgPtr = setupBuffer;
	if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* class request type */

		switch (rq->bRequest)
		{
			case USBRQ_HID_GET_REPORT:
				/* wValue: ReportType (highbyte), ReportID (lowbyte) */
				return curGamepad->buildReport(setupBuffer, rq->wValue.bytes[0]);

			case USBRQ_HID_GET_IDLE:
				if (rq->wValue.bytes[0] > 0 && rq->wValue.bytes[0] <= MAX_REPORTS) {
					usbMsgPtr = idleRates + (rq->wValue.bytes[0] - 1);
					return 1;
				}
				break;

			case USBRQ_HID_SET_IDLE:
				if (rq->wValue.bytes[0]==0) {
					for (i=0; i<MAX_REPORTS; i++)
						idleRates[i] = rq->wValue.bytes[1];
				}
				else {
					if (rq->wValue.bytes[0] > 0 && rq->wValue.bytes[0] <= MAX_REPORTS) {
						idleRates[rq->wValue.bytes[0]-1] = rq->wValue.bytes[1];
					}
				}
				break;
		}
	}else{
	/* no vendor specific requests implemented */
	}
	return 0;
}


int main(void)
{
	char must_report = 0, first_run = 1;
	uchar idleCounters[MAX_REPORTS];
	int i;

	memset(idleCounters, 0, MAX_REPORTS);
	memset(idleRates, 0, MAX_REPORTS); // infinity

	/* Dip switch common: DB0, outputs: DB1 and DB2 */
	DDRB |= 0x01;
	DDRB &= ~0x06; 
	
	PORTB |= 0x06; /* enable pull up on DB1 and DB2 */
	PORTB &= ~0x01; /* Set DB0 to low */

	_delay_ms(10); /* let pins settle */


			curGamepad = snesGetGamepad();


	// configure report descriptor according to
	// the current gamepad
	rt_usbHidReportDescriptor = curGamepad->reportDescriptor;
	rt_usbHidReportDescriptorSize = curGamepad->reportDescriptorSize;

	if (curGamepad->deviceDescriptor != 0)
	{
		rt_usbDeviceDescriptor = (void*)curGamepad->deviceDescriptor;
		rt_usbDeviceDescriptorSize = curGamepad->deviceDescriptorSize;
	}
	else
	{
		// use descriptor from devdesc.c
		//
		rt_usbDeviceDescriptor = (void*)usbDescrDevice;
		rt_usbDeviceDescriptorSize = getUsbDescrDevice_size();
	}

	// patch the config descriptor with the HID report descriptor size
	my_usbDescriptorConfiguration[25] = rt_usbHidReportDescriptorSize;

	//wdt_enable(WDTO_2S);
	hardwareInit();
	set_sleep_mode(SLEEP_MODE_IDLE);

	curGamepad->init();
	

	odDebugInit();
	usbInit();
	sei();
	DBG1(0x00, 0, 0);

	
	for(;;){	/* main event loop */
		wdt_reset();

		// this must be called at each 50 ms or less
		usbPoll();

		if (first_run) {
			curGamepad->update();
			first_run = 0;
		}

		if (curGamepad->ultraPoll)
			curGamepad->ultraPoll();

		/* Try to report at the granularity requested by
		 * the host */
#if defined(__AVR_ATmega8__)
		//If using Atmega8 the registers are different so need tweaking
		if(TIFR & (1<<TOV0))  /* 22 ms timer */
		{
			TIFR = 1<<TOV0;
#else
		if(TIFR0 & (1<<TOV0))  /* 22 ms timer */
		{ 
			TIFR0 = 1<<TOV0;
#endif

			for (i=0; i<curGamepad->num_reports; i++) 
			{
				if(idleRates[i] != 0)
				{
					if(idleCounters[i] > 4){
						idleCounters[i] -= 5;   /* 22 ms in units of 4 ms */
					}else{
						// reset the counter and schedule a report for this
						idleCounters[i] = idleRates[i];
						must_report |= (1<<i);
					}
				}
			}
		}

		/* Read the controller periodically*/
#if defined(__AVR_ATmega8__)
		//If using Atmega8 the registers are different so need tweaking
		if ((TIFR & (1<<OCF2)) )
		{
			TIFR = 1<<OCF2;
#else
		if ((TIFR2 & (1<<OCF2A)) )
		{
			TIFR2 = 1<<OCF2A;

#endif

			//C1 turns high
			PORTC |= 0x02;

			// Ok, the timer tells us it is time to update
			// the controller status. 
			//
			// But wait! USB triggers interrupts at a rate of approx. 1ms. 
			// Waiting until an interrupt has just been serviced before attempting
			// to update the controller prevents USB interrupt servicing 
			// delays from messing with the timing in the controller update 
			// function. 
			//
			// Even though what I am preventing here has never been, as far as
			// I know, a problem with the kind of controllers supported by this
			// circuit, it can't be bad. And at least, in theory, it prevents
			// retries in the SEGA Paddle code.
			//
			// The trick here is to put the CPU in idle mode ; That is, wating
			// for interrupts, doing nothing. When the CPU resumes, an interrupt
			// has been serviced. The final delay helps when we get a burst of
			// interrupts.
	
			// don't do this for (currently) nes.c
			if (!curGamepad->ultraPoll)
			{
				sleep_enable();
				sleep_cpu();
				sleep_disable();
				_delay_us(100);
			}

			curGamepad->update();

			/* Check what will have to be reported */
			for (i=0; i<curGamepad->num_reports; i++) {
				if (curGamepad->changed(i+1)) {
					must_report |= (1<<i);
				}
			}

			//C1 turns off
			PORTC &= ~0x02;
		}
			
		if(must_report)
		{
			//C0 turns high
			PORTC |= 0x01;

			for (i=0; i<curGamepad->num_reports; i++)
			{
				if ((must_report & (1<<i)) == 0)
					continue;

				if (usbInterruptIsReady())
				{
					char len;

					len = curGamepad->buildReport(reportBuffer, i+1);
					usbSetInterrupt(reportBuffer, len);

					while (!usbInterruptIsReady())
					{
						usbPoll();
						wdt_reset();
						if (curGamepad->ultraPoll)
							curGamepad->ultraPoll();
					}
				}

//				must_report &= ~(1<<i);
			}
				
			must_report = 0;

			//C0 turns low
			PORTC &= ~0x01;

		}
	}
	return 0;
}

