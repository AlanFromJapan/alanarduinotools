/*
 * DrVetinaryClock.c
 *
 * Created: 2013/02/16 18:11:19
 *  Author: Alan
 */ 

//#define F_CPU 9600000

#include <avr/io.h>
#include <avr/delay.h>

int main(void)
{
	//default uses the 9.6MHz internal crystal ... don't need so much speed	
	//CKSEL |= 0x03; //internal oscillator
	
	CLKPR = (0<<CLKPCE); // & (1 << CLKPS3);
	
	
	//all ports output!
	DDRB = 0xFF;
	
	uint8_t vPort = (1 << PB4) | (0 << PB3);
	
	while (1){
		_delay_ms(1000);
		
		PORTB = vPort;
		_delay_ms(15);
		PORTB = 0x00;

		//reverse status of PB3 and PB4 : the electro-magnet must change direction each time
		vPort ^= (1 << PB4) | (1 << PB3);
				
	
	}
}