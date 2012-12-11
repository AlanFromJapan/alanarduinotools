/*
 * WhatAWatt.cpp
 *
 * Created: 2012/12/11 23:13:04
 *  Author: Alan
 */ 


#define F_CPU 8000000

#include <avr/io.h>
#include "Rapide7seg.h"


//Read ADC value into a 16 bits uint16
uint16_t ReadADCx16()
{	
	//stop all digit display (to avoid noise)
	PORTD = 0xff;
	
	ADCSRA |= (1 << ADSC); // Start conversion
	while (ADCSRA & (1 << ADSC)); // wait until conversion is done

	uint16_t v = ADCL;
	v = (ADCH << 8) + v;

	
	return v;
}

void InitADC(){
	/*********************************************************************/
	//ADC setup
	//ADC5, right adjust, using aref
	//ADMUX = 0x00;
	ADMUX = 0x05 | (1 << REFS0) | (0 << ADLAR);
	
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
	
	/*********************************************************************/
	
}

int main(void)
{
	setup7seg();
	InitADC(); 
	
    while(1)
    {
		/*
		for (uint16_t vNumber = 0; vNumber < 1000; vNumber++){
			uint16_t vVal = vNumber * 10;
			showNumber(vVal, 0, 2);
		}
		*/
		
		showNumber(ReadADCx16() *10, 0, 2);
    }
}