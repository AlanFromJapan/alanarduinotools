/*
 * Pullups.c
 *
 * Created: 2012/08/01 22:49:11
 *  Author: Alan
 */ 

#include <avr/io.h>

void TestPullups(){
	
	//D goes output (drive the led) - initially off
	DDRD = 0xFF;
	PORTD = 0x00;
	
	//A goes input, with pullups : care for the order DDRx -> PORTx -> MCUCR
	DDRA = 0x00;
	PORTA = 0xFF; //pull ups
	MCUCR |= (0 << PUD); //make sure pullups are not disabled
	
	while (1){
		
		//read the pinA and turn on the led on port D
		if ((PINA & (1 << PINA1)) == 0x00){
			
			PORTD = 0x00;
		}
		else {
			PORTD = 0xFF;
		}
	}
}