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
/* Make keyboard report to send to USB                                  */
/************************************************************************/
void buildKeyboardReport(uint8_t pKey) {
	//default: nothing
	keyboard_report.modifier = 0;
	keyboard_report.keycode[0] = 0;
	
	//if caps, set the modifier and shift the character to the lchars
	if(pKey >= 'A' && pKey <= 'Z'){
		keyboard_report.modifier = MOD_SHIFT_LEFT;
		pKey = pKey + ('a' - 'A');
	}
	
	if(pKey >= 'a' && pKey <= 'z'){
		keyboard_report.keycode[0] = 4+(pKey-'a');
		return;
	}				

	//Digits
	if(pKey >= '1' && pKey <= '9'){
		keyboard_report.keycode[0] = 30+(pKey-'1');
		return;
	}
	else{
		if(pKey == '0'){
			keyboard_report.keycode[0] = KEY_0_PARC;
			return;
		}
	}

	//The rest
	switch (pKey){
		case '!': keyboard_report.keycode[0] = KEY_1_EXCL; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '@': keyboard_report.keycode[0] = KEY_2_AT; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '#': keyboard_report.keycode[0] = KEY_3_SHARP; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '$': keyboard_report.keycode[0] = KEY_4_DOL; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '%': keyboard_report.keycode[0] = KEY_5_PCNT; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '^': keyboard_report.keycode[0] = KEY_6_HAT; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '&': keyboard_report.keycode[0] = KEY_7_AMP; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '*': keyboard_report.keycode[0] = KEY_8_STAR; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '(': keyboard_report.keycode[0] = KEY_9_PARO; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case ')': keyboard_report.keycode[0] = KEY_0_PARC; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '-': keyboard_report.keycode[0] = KEY_DASH_UNDERSCORE; break;
		case '_': keyboard_report.keycode[0] = KEY_DASH_UNDERSCORE; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '=': keyboard_report.keycode[0] = KEY_EQUAL_PLUS; break;
		case '+': keyboard_report.keycode[0] = KEY_EQUAL_PLUS; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case ';': keyboard_report.keycode[0] = KEY_SEMIC_COLON; break;
		case ':': keyboard_report.keycode[0] = KEY_SEMIC_COLON; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case ',': keyboard_report.keycode[0] = KEY_COMA_LT; break;
		case '<': keyboard_report.keycode[0] = KEY_COMA_LT; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '.': keyboard_report.keycode[0] = KEY_DOT_GT; break;
		case '>': keyboard_report.keycode[0] = KEY_DOT_GT; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		case '/': keyboard_report.keycode[0] = KEY_SLASH_QUEST; break;
		case '?': keyboard_report.keycode[0] = KEY_SLASH_QUEST; keyboard_report.modifier = MOD_SHIFT_LEFT; break;
		
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

	
	//4: buttons
	//button input
	//PC1 in
	DDRC &= ~0x01;
	//Pull up on PC1
	PORTC = (1 << PORTC1);
	//just make sure pullups are NOT disabled
	MCUCR |= (0 << PUD);
	
	uchar* str = "hello Monde. !#$%&'()<>?:;";
	
	for(;;){                // main event loop
		//USB data pull
		wdt_reset();
		usbPoll();
	
	
		//Other stuffs to do in main loop
		//check for button press
		if ((~PINC & (1 << PINC1)) != 0){
		
			//1) Send the character
			if(usbInterruptIsReady()){
				buildKeyboardReport('a');			
				usbSetInterrupt(&keyboard_report, sizeof(keyboard_report));
				_delay_ms(50);

				buildKeyboardReport('B');
				usbSetInterrupt(&keyboard_report, sizeof(keyboard_report));
				_delay_ms(50);

				buildKeyboardReport('c');
				usbSetInterrupt(&keyboard_report, sizeof(keyboard_report));
				_delay_ms(50);

				buildKeyboardReport('Z');
				usbSetInterrupt(&keyboard_report, sizeof(keyboard_report));
				_delay_ms(50);
						
				//DON'T FORGET to stop sending character
				buildKeyboardReport(NULL);			
				usbSetInterrupt(&keyboard_report, sizeof(keyboard_report));
				_delay_ms(50);
			}
		
		
		}
	
	}

return 0;
}