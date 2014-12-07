/*
 * Atmegaxx8.c
 *
 * Created: 2012/02/21 23:52:01
 *  Author: Alan
 */ 

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB = 0xFF;
	//DDRD = 0x00;
	PORTB =0x00;
	
	DDRD = 0xFF;
	
    while(1)
    {
        PORTD ^= 0xFF;
		_delay_ms(500);
		
    }
}