/*
 * KodiRemote_Receiver.c
 *
 * Created: 2016/02/23 19:40:07
 *  Author: Alan
 */ 

//defined in the compilation params "-D F_CPU=12000000"
//#define F_CPU 12000000



#include <avr/io.h>
#include <util/delay.h>     // for _delay_ms()
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>


#include "Keyboard.h"
#include "usbdrv.h"
#include "vusb-config.h"
#include "serialComm.h"

static void usbHardwareInit(void)
{
	uint8_t	i, j;

	PORTD = 0xfa;   /* 1111 1010 bin: activate pull-ups except on USB lines */
	DDRD = 0x07;    /* 0000 0111 bin: all pins input except USB (-> USB reset) */
	j = 0;
	while(--j){     /* USB Reset by device only required on Watchdog Reset */
		i = 0;
		while(--i); /* delay >10ms for USB reset */
	}
	DDRD = 0x02;    /* 0000 0010 bin: remove USB reset condition */
	/* configure timer 0 for a rate of 12M/(1024 * 256) = 45.78 Hz (~22ms) */
	//AVI: what is this used for? 
	TCCR0 = 5;      /* timer 0 prescaler: 1024 */
}

static void usbPurgeEvents(){
	do {
		//usb data pull
		wdt_reset();
		usbPoll();
		_delay_ms(3);
	} while (!usbInterruptIsReady());
}

#define USB_DELAY_DELTA_MS 50
static void usbDelayMs(double pDelay){
	uint8_t vDelay = (uint8_t)((uint16_t)pDelay / USB_DELAY_DELTA_MS);
	for (uint16_t t = 0; t < vDelay; t++){
		_delay_ms(USB_DELAY_DELTA_MS);
		//Keep USB connection alive
		usbPurgeEvents();
	}
}
	
//Sends a null terminated string as a keyboard
static void sendString (char* pStr){
	for (uint8_t* p = pStr; (*p) != 0; p++){

		//Necessary: without that if you send a few characters only the first 5-8 ones will arrive
		usbPurgeEvents();
					
		//1) Send the character
		buildKeyboardReport(*p);
		usbSetInterrupt(&keyboard_report, sizeof(keyboard_report));
		_delay_ms(20);
					
		//2) DON'T FORGET to stop sending character FOR EACH Character
		buildKeyboardReport(NULL);
		usbSetInterrupt(&keyboard_report, sizeof(keyboard_report));
		_delay_ms(20);
	}
				
}
	

/************************************************************************/
/* Main method                                                          */
/************************************************************************/
int main(void)
{
    wdt_enable(WDTO_1S);  // watchdog status is preserved on reset

	//1: Init USB
    usbHardwareInit();
    
    usbInit();
    usbDeviceDisconnect();

    // fake USB disconnect for > 250 ms
    for( uint8_t i=255; i>0; i-- ) {
	    wdt_reset();
	    _delay_ms(1);
    }
    usbDeviceConnect();

    sei();

	//2: Now USB is up, init the Serial
//	serialHardwareInit();
	
	//3: Leds
	DDRB = 0xff; //PortB all output

	uint8_t v = 0;			
    for(;;){                
		//Keep USB connection alive
		usbPurgeEvents();
		
		PORTB = 1 << (v % 4);	
		usbDelayMs(300);
		
		v++;
    }	
	
	return 0;
}