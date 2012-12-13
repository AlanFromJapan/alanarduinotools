/*
 * WhatAWatt.cpp
 *
 * Created: 2012/12/11 23:13:04
 *  Author: Alan
 */ 


#define F_CPU 8000000

#include <avr/io.h>
#include <avr/interrupt.h>
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

//Setup the timer to read in the background
void setupTimer0(){
	// Prescaler = FCPU/64
	// 8MHz / 64 / 256bits => overflow about every 2ms
	//TCCR0B |= (1<<CS01)|(1<<CS00);

	// Prescaler = FCPU/256
	// 8MHz / 256 / 256bits => overflow about every 8ms
	TCCR0B |= (1<<CS02);

	
	//overflow of timer 0
	TIMSK0 |= (1 << TOIE0);
	
	//reset counter to zero
	TCNT0 = 0x00;
}

//number of average values
#define AVERAGE_MAX 15
#define OVERFLOW_COUNT 20
#define OVERFLOW_COUNT_NEW_AVG (AVERAGE_MAX * OVERFLOW_COUNT)
volatile uint16_t	mAverageTab[AVERAGE_MAX] ;
volatile uint8_t	mAverageIndex = 0;
volatile uint16_t	mMaxVoltage = 0;
volatile uint16_t	mOverflowCount = 0;
volatile float		mLatestPower = 0;
	
//Timer 0 overflow interrupt
//Triggered about 488 times a sec
ISR(TIMER0_OVF_vect){
	uint16_t vValue = ReadADCx16();
	mMaxVoltage = (vValue > mMaxVoltage ? vValue : mMaxVoltage);
	
	mOverflowCount++;
	
	//more than 20 times => scanned max for 20 x 2ms = 40ms (120Hz)
	if (mOverflowCount >= OVERFLOW_COUNT){
		//get a value, read many times to normally read a MAX local (the top of the AC sine)
		mAverageTab[mAverageIndex] = mMaxVoltage;
		mAverageIndex = (mAverageIndex + 1) % AVERAGE_MAX; //tab length	
		
		//vMaxCurrent= 0;
	}
	
	//got 5 readings, update the average to display
	if (mOverflowCount >= OVERFLOW_COUNT_NEW_AVG){
		float vAvgVoltage = 0;
		for(uint8_t i = 0; i < AVERAGE_MAX; i++){
			vAvgVoltage += mAverageTab[i];
		}
		vAvgVoltage = vAvgVoltage / (float)AVERAGE_MAX;
				
		//current and effective current are in mA
		float current=vAvgVoltage/1024.0*5.0/800.0*2000000.0;
		float effective_value=current/1.414;
		
		//Japan is 100 V, Watts = mA/ 1000 x 100V = mA /10
		//BUT I use only 3 x 7 segs aligned to the left, so I need to shift the value 1 digit to the left => x10
		//CCL : Watts = value mA /10 x 10 = value mA
		mLatestPower = effective_value;
		
		mOverflowCount = 0;
		mMaxVoltage= 0;
	}
}


int main(void)
{
	//factory settings is to divide internal clock 8MHz by 8.
	//don't, and just run at 8 MHz (set the clock divider to 1 so no effect)
	CLKPR = (1<<CLKPCE);
	CLKPR = 0; // Divide by 1
		
	setup7seg();
	InitADC(); 
	setupTimer0();
	
	//start the interrupts
	sei();
	
    while(1)
    {
		showNumber((uint16_t)(mLatestPower), 0, 2, false);		
    }
}


