/* Name: main.c
 * Project: TricolorUsbSignal http://kalshagar.wikispaces.com/Tricolor+USB+alert
 * Author: AlanFromJapan (Dec 2014)
 * Source Project: ThingM.Blink(1)
 *
 * blink(1) - BlinkM-like device with USB interface
 *  2012, Tod E. Kurt, http://thingm.com/ , http://todbot.com/blog/
 *
 * Based on much code from:
 *   Project: hid-custom-rq example by  Christian Starkjohann
 *   LinkM : http://linkm.thingm.com/
 *
 *
 * Note: blink(1) contains no code from BlinkM.  The circuit is different,
 * the PWM is hardware, not software, and the fading and pattern engine work
 * differently.
 */

#define F_CPU 12000000
#include <avr/io.h>
#include <util/delay.h>     // for _delay_ms()
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include "usbdrv.h"
#include "Keyboard.h"
#include "vusb-config.h"

/* ----------------------- hardware I/O abstraction ------------------------ */

/* pin assignments:
PB0	Key 1
PB1	N/A
PB2	N/A
PB3 N/A
PB4	N/A
PB5 red LED
PB6 yellow LED
PB7	green LED

PA0 N/A
PA1 N/A

PD0	N/A
PD1	debug tx ?
PD2	USB+ (int0)
PD3	N/A
PD4	USB-
PD5	N/A
PD6	N/A
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

int main(void) {
    wdt_enable(WDTO_1S);  // watchdog status is preserved on reset


    hardwareInit();
    
    usbInit();
    usbDeviceDisconnect();

    // fake USB disconnect for > 250 ms
    for( uint8_t i=255; i>0; i-- ) {
	    wdt_reset();
	    _delay_ms(1);
    }
    usbDeviceConnect();

    sei();

    for(;;){                // main event loop
		wdt_reset();
		usbPoll();
    }	
	
	return 0;
}
