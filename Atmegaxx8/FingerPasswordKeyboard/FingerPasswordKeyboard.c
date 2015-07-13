/*
 * FingerPasswordKeyboard.c
 *
 * Created: 2015/06/17 22:44:16
 *  Author: Alan
 */ 

//defined in the compilation params "-D F_CPU=12000000"
//#define F_CPU 12000000



#include <avr/io.h>
#include <util/delay.h>     // for _delay_ms()
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include "usbdrv.h"
#include "Keyboard.h"
#include "vusb-config.h"
#include "serialComm.h"
#include "FPS.h"

static void usbHardwareInit(void)
{
	uint8_t	i, j;

	PORTB = 0x00;   //no pullups
	DDRB = 0xff;    //all outputs

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
	serialHardwareInit();
	
	//3: open communication to FPS
	fpsInit();
	
	uint32_t vSpeedDown = 0;
	uint8_t vToggle = 0;
    for(;;){                // main event loop
		//USB data pull
		wdt_reset();
		usbPoll();
		
		
		//Other stuffs to do in main loop
		vSpeedDown++;		
		if (vSpeedDown >= 500000){
			vSpeedDown = 0;
			
			fpsSetLight(vToggle);
			
			//vToggle = !vToggle;
			if (vToggle == 0)
				vToggle = 1;
			else
				vToggle = 0;
		}
		
    }	
	
	return 0;
}