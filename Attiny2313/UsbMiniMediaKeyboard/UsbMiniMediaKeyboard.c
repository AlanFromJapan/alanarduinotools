/*
* UsbVolumeControl.c
*
* Created: 2016/12/04 16:29:25
*  Author: Alan
* http://electrogeek.cc/usb%20volume%20control.html
* 
* Based on the works of Vitalij found at http://obruboff.ru/english-pages/usb-volume-control/
*
*/

// Set the fuses with "avrdude -pt2313 -cavrispmkII -u  -Ulfuse:w:0xce:m -Uhfuse:w:0xdf:m -Uefuse:w:0xff:m"
// Will set the fuse to use ISP, no divide clock and external crystal (12MHz is what you should use)


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

//Key values to send to the host https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf p77-78
#define PLAY_PAUSE 	0xcd
#define PLAY_NEXT	0xb5

// LEDS on PB3 & PB4
#define LED_1_MASK 	0x08
#define LED_2_MASK 	0x10

#define LEDS_MASK	(LED_1_MASK | LED_2_MASK)


// Buttons on PB0 & PB1
#define KEY_PAUSE_MASK	0x01
#define KEY_NEXT_MASK	0x02

#define KEYS_MASK		(KEY_PAUSE_MASK | KEY_NEXT_MASK)



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

#define USB_DELAY_DELTA_MS ((uint16_t)50)
static void usbDelayMs(uint16_t pDelay){
	uint8_t vDelay = (uint8_t)(pDelay / USB_DELAY_DELTA_MS);
	for (uint16_t t = 0; t < vDelay; t++){
		_delay_ms(USB_DELAY_DELTA_MS);
		//Keep USB connection alive
		usbPurgeEvents();
	}
}

static inline void ledsOn(uint8_t pLedMask){
	PORTB |= pLedMask;
}

static inline void ledsOff(){
	PORTB &= ~(LEDS_MASK);
}

/************************************************************************/
/* MAIN                                                                 */
/************************************************************************/
int main(void) {

	uchar KeyPressed = 0;

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
	            
	reportBuffer[0] = 1;  // ReportID = 1
	reportBuffer[2] = 0;  
	
	//----------------------------------

	//2: 3 tact switch setup (pullups)
	//PB0-1 input
	DDRB &= ~(KEYS_MASK);

	//Pull up on PB0-2
	PORTB |= KEYS_MASK;
		
	//just make sure pullups are NOT disabled
	MCUCR |= (0 << PUD);

	
	//3: LED
	DDRB |= LEDS_MASK;
	//all leds off
	ledsOff();

	
	//MAIN LOOP
	for(;;){
		usbPoll();

		//nothing pressed at the start
		KeyPressed = 0x00;

		//check buttons status (buttons are pulled-up and when pressed they are tied to GND) so invert PINB status
		if (((~PINB) & KEY_PAUSE_MASK) == KEY_PAUSE_MASK){
			KeyPressed = PLAY_PAUSE;
			ledsOn(LED_1_MASK);
		}
		else{
			if (((~PINB) & KEY_NEXT_MASK) == KEY_NEXT_MASK){
				KeyPressed = PLAY_NEXT;
				ledsOn(LED_2_MASK);
			}
		}

		//if something was pressed
		if (KeyPressed != 0x00) {


			//send the USB message
			if (usbInterruptIsReady()){
				//Send the key
				reportBuffer[1] = KeyPressed;
				usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
				_delay_ms(10);

				//and send STOP!!
				KeyPressed = 0x00;
				reportBuffer[1] = KeyPressed;
				usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
			}

			//wait a little to debounce on the cheap
			usbDelayMs(250);
		}

		ledsOff();
	}
	
	return 0;	
}
