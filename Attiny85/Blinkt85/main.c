/*
 * main.c
 *
 *  Created on: Mar 21, 2024
 *      Author: alan
 */

#include <avr/io.h>
#include <util/delay.h>

int main(){
	//all output
	DDRB = 0xff;
	//off
	PORTB =0x00;


    while(1)
    {
    	//toggle
        PORTB ^= 0xFF;
		_delay_ms(500);

    }


	return 0;
}
