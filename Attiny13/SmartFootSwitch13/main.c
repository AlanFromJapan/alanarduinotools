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
	PORTB &= ~(1 << PORT_LED_STATUS);

	_delay_ms(50);

	PORTB |= 1 << PORT_LED_STATUS;
	PORTB &= ~(1 << PORT_LED_CLAP);

	_delay_ms(50);

	PORTB &= ~(1 << PORT_LED_CLAP);
	PORTB &= ~(1 << PORT_LED_STATUS);
}

//Toggles the relay status
void toggleRelay (){
	PORTB ^= (1 << PORT_RELAY);
}

int main(void)
{
	//no diviser on clock
	// Set the clock change enable bit (4 cycles to do the change)
	CLKPR = (1<<CLKPCE);
	// no clock pre-scaling
	CLKPR = 0x00;

	//Switch on PB0 only is input, the rest it output
	DDRB = 0xFE;
	//pullup on PB0
	PORTB = 0x01;

	//just make sure pullups are NOT disabled
	MCUCR |= (0 << PUD);

	//main body loop
	while(1) {
		//if foot button pressed
		if ((PINB & 0x01) != 0x01){
			blink();

			toggleRelay();

			//debounce
			_delay_ms(100);
		}

	}
}


