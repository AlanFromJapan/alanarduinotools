/*
 * UsbKeyboardSample.c
 *
 * Created: 2015/07/19 22:44:00
 *  Author: Alan
 * 
 * An enormous thank you to http://codeandlife.com/2012/06/18/usb-hid-keyboard-with-v-usb/ who made this ultimately simple.
 * Not sure my code is different so much, but it's mine so /better/ (NIH syndrome advanced stage)
 *
 * See my doc here http://kalshagar.wikispaces.com/USB+without+Arduino
 * 
 */ 

//defined in the compilation params "-D F_CPU=12000000" or define it here
#define F_CPU 12000000



#include <avr/io.h>
#include <util/delay.h>     // for _delay_ms()
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include "usbdrv.h"
#include "Keyboard.h"
#include "vusb-config.h"

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

	
	//4: buttons
	//button input
	//PC1 in
	DDRC &= ~0x01;
	//Pull up on PC1
	PORTC = (1 << PORTC1);
	//just make sure pullups are NOT disabled
	MCUCR |= (0 << PUD);
	
	
	for(;;){                // main event loop
	//USB data pull
	wdt_reset();
	usbPoll();
	
	
	//Other stuffs to do in main loop
	//check for button press
	if ((~PINC & (1 << PINC1)) != 0){
		
		//1) Send the character
		if(usbInterruptIsReady()){
			keyboard_report.keycode[0] = 0; //modifier
			keyboard_report.keycode[1] = KEY_A; //key
			
			usbSetInterrupt(&keyboard_report, sizeof(keyboard_report));
			_delay_ms(50);
		}
		
		//2) Send 0 to notify key unpressed (otherwise endless aaaaaaaaaaaaaaaaaa...)
		if(usbInterruptIsReady()){
			keyboard_report.keycode[0] = 0; //modifier
			keyboard_report.keycode[1] = 0; //key released
			
			usbSetInterrupt(&keyboard_report, sizeof(keyboard_report));
			_delay_ms(50);
		}
		
	}
	
}

return 0;
}