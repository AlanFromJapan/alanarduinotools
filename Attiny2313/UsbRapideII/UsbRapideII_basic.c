/*
* UsbRapideII_basic.c
*
* Created: 2020/03/19
*  Author: Alan
* http://electrogeek.cc/
*
*/

//defined in the compilation params "-D F_CPU=12000000" or define it here
//#define F_CPU 12000000

#include <avr/io.h>
#include <avr/iotn2313.h> //because couldn't find TCCR0B register in usbHardwareInit()
#include <util/delay.h>     // for _delay_ms()
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include "usbdrv.h"
#include "vusb-config.h"


#define LED2_REG	DDRB
#define LED2_PORT	PORTB
#define LED2_R		0x02
#define LED2_G		0x08
#define LED2_B		0x04
#define LED2_MASK	(LED2_R | LED2_G | LED2_B)


static void usbHardwareInit(void) {
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
	//AVI: set the Timer 0 prescaler to clock / 1024 - other bits are fine with a 0 so do a brutal assignment of the whole register
	TCCR0B = 5;      /* timer 0 prescaler: 1024 */
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

static inline void ledsOn(){
	LED2_PORT |= LED2_MASK;
}

static inline void ledsOff(){
	LED2_PORT &= ~(LED2_MASK);
}

static inline void red2(uint8_t pVal){
	if (pVal == 0){
		//OFF
		LED2_PORT &= ~(LED2_R);
	}
	else {
		//ON
		LED2_PORT |= LED2_R;
	}
}

static inline void green2(uint8_t pVal){
	if (pVal == 0){
		//OFF
		LED2_PORT &= ~(LED2_G);
	}
	else {
		//ON
		LED2_PORT |= LED2_G;
	}
}

static inline void blue2(uint8_t pVal){
	if (pVal == 0){
		//OFF
		LED2_PORT &= ~(LED2_B);
	}
	else {
		//ON
		LED2_PORT |= LED2_B;
	}
}

static inline void setColor2(uint8_t pVal){
	pVal = pVal & 0x0e;
	LED2_PORT &= ~LED2_MASK;
	LED2_PORT |= pVal;
}

/************************************************************************/
/* MAIN                                                                 */
/************************************************************************/
int main(void) {

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
	            
	
	//----------------------------------

	LED2_REG |= LED2_MASK;
	ledsOff();

	//just make sure pullups are NOT disabled
	MCUCR |= (0 << PUD);
	
	//MAIN LOOP
	for(;;){
		usbPoll();

		blue2(0);
		red2(1);
		usbDelayMs(100);
		red2(0);
		green2(1);
		usbDelayMs(100);
		green2(0);
		blue2(1);
		usbDelayMs(100);
	}
	
	return 0;	
}
