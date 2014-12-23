/* Name: main.c
 * Project: TricolorUsbSignal http://kalshagar.wikispaces.com/Tricolor+USB+alert
 * Author: AlanFromJapan (Dec 2014)
 * Source Project: HID-Test by Christian Starkjohann
 * License: no license, enjoy.
 */

#define F_CPU 12000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include "usbdrv.h"
#include "Keyboard.h"
#include "vusb-config.h"

/* ----------------------- hardware I/O abstraction ------------------------ */

/* pin assignments:
PB0	Key 1
PB1	Key 2
PB2	Key 3
PB3	Key 4
PB4	Key 5
PB5 red LED
PB6 yellow LED
PB7	green LED

PA0 
PA1

PD0	USB-
PD1	debug tx
PD2	USB+ (int0)
PD3	Key 13
PD4	Key 14
PD5	Key 15
PD6	Key 16
*/

static void hardwareInit(void)
{
uint8_t	i, j;

    PORTB = 0x1f;   /* activate all pull-ups EXCEPT on leds */
    DDRB = 0xe0;       /* all pins input EXCEPT on leds that are outputs */
    
    PORTD = 0xfa;   /* 1111 1010 bin: activate pull-ups except on USB lines */
    DDRD = 0x07;    /* 0000 0111 bin: all pins input except USB (-> USB reset) */
	j = 0;
	while(--j){     /* USB Reset by device only required on Watchdog Reset */
		i = 0;
		while(--i); /* delay >10ms for USB reset */
	}
    DDRD = 0x02;    /* 0000 0010 bin: remove USB reset condition */
    /* configure timer 0 for a rate of 12M/(1024 * 256) = 45.78 Hz (~22ms) */
    TCCR0A = 5;      /* timer 0 prescaler: 1024 */
}

/* ------------------------------------------------------------------------- */


//Methods that returns the key pressed to the host
inline void buildReportEmpty()
{
	//always answers "no character pressed"
	reportBuffer[0] = 0;
	reportBuffer[1] = 0;

}

/* ------------------------------------------------------------------------- */

void main(void) {
	wdt_enable(WDTO_2S);
	
	hardwareInit();
	usbInit();
	sei();

	while (1) {	// main event loop
		//dirty popular trick to avoid being shot by the watchdog
		wdt_reset();
		
		//process USB events
		usbPoll();
		
		//Never send anything: reception (HOST -> current device) is done in the usbFunctionSetup() and usbFunctionWrite()
	}
	
}
