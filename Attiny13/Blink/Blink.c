/*
 * Blink.c
 *
 * Created: 2012/02/11 23:04:15
 *  Author: Alan
 */ 

#define F_CPU 8000000L

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	//factory settings is to divide internal clock 8MHz by 8.
	//don't, and just run at 8 MHz (set the clock divider to 1 so no effect)
	CLKPR = (1<<CLKPCE);
	CLKPR = 0; // Divide by 1 

	//port B pins 0-5 go OUTPUT
    DDRB = 0xFF;
	/* Set Fast PWM mode. */
	TCCR0A |= (1<<WGM00) | (1<<WGM01);
	/* Clear 0C0A on compare. */
	TCCR0A |= (1<<COM0A1);
	/* Start timer, no prescaling. */
	TCCR0B |= (1<<CS00);
	//initial PWM value is 0
	OCR0A = 0x00;
 
	while (1){
		for (int i=0; i < 255; i ++){
			OCR0A = i;
			_delay_ms(5);
		}
		
		for (int i=0; i < 255; i ++){
			OCR0A = 255-i;
			_delay_ms(5);
		}
		
	}
	
}