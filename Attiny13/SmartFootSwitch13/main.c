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


//#define F_CPU 4800000L

#include <avr/io.h>
#include <avr/iotn13.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

#define PORT_LED_CLAP PORTB3
#define PORT_LED_STATUS PORTB4
#define PORT_RELAY PORTB2

//Just for the test of leds ... will be removed by optimizer
void test_Blink()
{

	//all out
	//DDRB = 0xFF;

    while(1)
    {
		PORTB = 1 << PORT_LED_CLAP;

		_delay_ms(50);

		PORTB = 1 << PORT_LED_STATUS;

		_delay_ms(50);

		PORTB = 0;

		_delay_ms(900);
    }
}




int main(void)
{
	//no diviser on clock
	// Set the clock change enable bit (4 cycles to do the change)
	CLKPR = (1<<CLKPCE);
	// no clock pre-scaling
	CLKPR = 0x00;

	//all pins are output EXCEPT PB0 & PB1 (AIN0 and AIN1) that are input
	DDRB = 0xFF
		& ~(1 << PB0)
		& ~(1 << PB1);

	//needed it seems.
	//One need also a pulldown on the AIN0 (Comparator+) to avoid having it floating (if you use a switch for instance)
	PORTB &= ~(1<<PB0);    // no Pull-up on PB0

	//relay and status led are OFF
	PORTB &= ~(1 << PORT_RELAY) & ~(1 << PORT_LED_STATUS);


	//main body loop
	while(1) {
		test_Blink();

	}
}


