/*
 * ADC13.c
 *
 * Created: 2016/05/07 18:12:33
 *  Author: Alan
 */ 
#include <avr/io.h>

//setup ADC as read on AD2 (PB4) with the max clock prescaler
void setupADC() {

	ADMUX |=
	(1 << ADLAR) // AD result store in (more significant bit in ADCH) : ADCH holds the 8 MSB like that: perfect for PWM
	| (1 << MUX1); // Choose AD input AD2 (BP 4)

	//divide main clock by 128 for better results
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}


//this is the QUICK version, returns the high bits only
uint8_t doADC(){
	ADCSRA |= (1 << ADEN); // Analog-Digital enable bit
	ADCSRA |= (1 << ADSC); // Discarte first conversion
	
	while (ADCSRA & (1 << ADSC)); // wait until conversion is done
	
	ADCSRA |= (1 << ADSC); // start single conversion
	
	while (ADCSRA & (1 << ADSC)) // wait until conversion is done
	
	ADCSRA &= ~(1<<ADEN); // shut down the ADC

	return ADCH;
}