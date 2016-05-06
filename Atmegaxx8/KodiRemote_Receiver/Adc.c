/*
 * Adc.c
 *
 * Created: 2016/02/23 21:36:47
 *  Author: Alan
 */ 


#include <avr/io.h>

//Read ADC value into a 16 bits uint16
uint16_t ReadADCx16() {
	//set right align one has to read ADCH and ADCL to get the value
	ADMUX &= ~(1 << ADLAR);

	ADCSRA |= (1 << ADSC); // Start conversion
	while (ADCSRA & (1 << ADSC)); // wait until conversion is done

	uint16_t v = ADCL;
	v = (ADCH << 8) + v;
	
	return v;
}


//Read ADC value into a 8 bits 
uint8_t ReadADCx8() {
	//set left align so highest bits are in ADCH
	ADMUX |= (1 << ADLAR);
	
	ADCSRA |= (1 << ADSC); // Start conversion
	while (ADCSRA & (1 << ADSC)); // wait until conversion is done

	return ADCH;
}

void InitADC(uint8_t pAdcPin) {
	//keep only the 3 lowest bits
	pAdcPin &= 0x07;
	
	//port C is input
	DDRC = 0x00;
	
	/*********************************************************************/
	//ADC setup
	//pAdcPin is the input ADC, right adjust, using aref
	//ADMUX = 0x00;
	ADMUX = pAdcPin | (1 << REFS0) | (0 << ADLAR);
	
	//ADC enabled
	//Clock div by 128 (highest definition)
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
	
	/*********************************************************************/
	
}