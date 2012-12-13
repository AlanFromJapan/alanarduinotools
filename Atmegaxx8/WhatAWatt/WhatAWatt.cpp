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

#define AVERAGE_MAX 5

int main(void)
{
	//factory settings is to divide internal clock 8MHz by 8.
	//don't, and just run at 8 MHz (set the clock divider to 1 so no effect)
	CLKPR = (1<<CLKPCE);
	CLKPR = 0; // Divide by 1
		
	setup7seg();
	InitADC(); 
	
	uint16_t vAverageTab[AVERAGE_MAX] ;
	uint8_t vAverageCount = 0;
	uint8_t vAverageIndex = 0;
	
    while(1)
    {
		/*
		for (uint16_t vNumber = 0; vNumber < 1000; vNumber++){
			uint16_t vVal = vNumber * 10;
			showNumber(vVal, 0, 2);
		}
		*/
		
		//showNumber(ReadADCx16() *10, 0, 2);
		
		//get a value, read many times to normally read a MAX local (the top of the AC sine)
		uint16_t vMaxCurrent = 0;
		for (uint16_t i = 0; i < 200; i++) {
			uint16_t vValue = ReadADCx16();
			vMaxCurrent = (vValue > vMaxCurrent ? vValue : vMaxCurrent);
			_delay_ms(2);
		}	
		
		vAverageTab[vAverageIndex] = vMaxCurrent;
		vAverageIndex = (vAverageIndex + 1) % AVERAGE_MAX; //tab length 
		vAverageCount = (1+vAverageCount >AVERAGE_MAX ?AVERAGE_MAX : 1+vAverageCount);
		
		float vAvgCurrent = 0;
		for(uint8_t i = 0; i < vAverageCount; i++){
			vAvgCurrent += vAverageTab[i];
		}
		vAvgCurrent = vAvgCurrent / (float)AVERAGE_MAX;
		
		//current and effective current are in mA
		float current=vAvgCurrent/1024.0*5.0/800.0*2000000.0;
		float effective_value=current/1.414;
		
		//Japan is 100 V, Watts = mA/ 1000 x 100V = mA /10
		//BUT I use only 3 x 7 segs aligned to the left, so I need to shift the value 1 digit to the left => x10
		//CCL : Watts = value mA /10 x 10 = value mA
		showNumber((uint16_t)(effective_value), 0, 2);		
    }
}