/*
 * main.c
 *
 *  Created on: Mar 27, 2026
 *      Author: alan
 */

#include <avr/io.h>
#include <util/delay.h>

int main () {
	DDRC = 0xff;

	while (1) {
		PORTC = 0x00;
		_delay_ms(500);
		PORTC = 0xff;
		_delay_ms(500);
	}

}
