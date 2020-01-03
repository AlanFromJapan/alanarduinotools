/*
 * main.c - SmartFootSwitch13
 *
 * 	Based on the ClapClap13 board, except you won't clap your hands but press a foot switch.
 * 	https://github.com/AlanFromJapan/alanarduinotools/tree/master/Attiny13
 *
 *  Created on: Dec 18, 2019
 *      Author: alan
 *
 *
 */


//#define F_CPU 9600000L

#include <avr/io.h>
#include <avr/iotn13.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define PORT_LED_CLAP 	PORTB3
#define PORT_LED_STATUS PORTB4
#define PORT_RELAY 		PORTB2
#define PORT_SWITCH		PORTB0

#define AUTOMATE_OFF 		0
#define AUTOMATE_LONGPRESS 	1
#define AUTOMATE_ON 		2

uint8_t mAutomate= AUTOMATE_OFF;

#define DEBOUNCE_DELAY	100
#define LONGPRESS_COUNT 5
//----------------------------------------------------------------------------

//Just for the test of leds ... will be removed by optimizer
void test_Blink()
{

	//all out
	//DDRB = 0xFF;

    while(1)
    {
    	blink();

		_delay_ms(900);
    }
}


//Blink all leds once
void blink(void) {
	PORTB |= 1 << PORT_LED_CLAP;

	_delay_ms(50);

	PORTB &= ~(1 << PORT_LED_CLAP);

	_delay_ms(50);
}


//Turns the relay OFF
void relayOff() {
	PORTB &= ~(1 << PORT_RELAY);
	PORTB &= ~(1 << PORT_LED_STATUS);
}

//Turns the relay ON
void relayOn (){
	PORTB |= (1 << PORT_RELAY);
	PORTB |= (1 << PORT_LED_STATUS);
}

int main(void)
{
	//no diviser on clock
	// Set the clock change enable bit (4 cycles to do the change)
	CLKPR = (1<<CLKPCE);
	// no clock pre-scaling
	CLKPR = 0x00;

	//pullup on PB0
	PORTB = 0x01;
	//Switch on PB0 only is input, the rest it output
	DDRB = 0xFE;


	//make sure pullups are NOT disabled
	MCUCR |= (0 << PUD);

	//make sure we start with a relay OFF
	relayOff();

	//temporize or the relay often starts closed (ON) which NOT what I want
	_delay_ms(500);
	blink();
	blink();
	blink();

	uint8_t mLongPressCount = 0;
	//main body loop
	while(1) {
		//if foot button pressed
		if ((PINB & 0x01) != 0x01){
			blink();

			switch (mAutomate) {
				case AUTOMATE_OFF:
					// OFF -> LONGPRESS
					mAutomate = AUTOMATE_LONGPRESS;
					mLongPressCount = 0;
					break;
				case AUTOMATE_ON:
					//ON -> OFF
					mAutomate = AUTOMATE_OFF;
					relayOff();
					break;
				case AUTOMATE_LONGPRESS:
					mLongPressCount ++;

					if (mLongPressCount > LONGPRESS_COUNT) {
						relayOn();
					}
					break;
			}
			//debounce
			_delay_ms(DEBOUNCE_DELAY);
		}
		else {
			//NOT PRESSED
			switch (mAutomate) {
				case AUTOMATE_LONGPRESS:
					// LONGPRESS -> OFF or ON

					if (mLongPressCount > LONGPRESS_COUNT) {
						mAutomate = AUTOMATE_ON;
					}
					else {
						mAutomate = AUTOMATE_OFF;
					}

					break;
			}
		}

	}
}


