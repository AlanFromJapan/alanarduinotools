/*
* RGBMatrix.cpp
*
* Created: 2013/01/05 18:54:14
*  Author: Alan
*/

//well in real it's more like 8MHz but this value makes a nice flickerless display so...
#define F_CPU 1000000L

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "RGBMatrix.h"
#include "SimpleAlgo.h"
#include "Font.h"








ISR(TIMER2_OVF_vect){
	//RandomColors();
	//Waves1();
	//WavesRandom();

	//NexusLike();

	if (mTiming == 400){
		mTiming = 0;
		mCount = (mCount >= 9 ? 0 : mCount+1);
		
		uint8_t vColor = rand() % 3;
		vColor = idToRGB(vColor);

		
		ShowOne(vColor, mCount);
	}
	mTiming++;
	
}

//inits timer 1 to do interrupt on overflow (calls ISR(TIMER2_OVF_vect))
void init_timer2_OVF() {
	
	//timer 2
	TCCR2 = 
		//No PWM
		(1 << FOC2) 
		//Divide by 64
		| (1 << CS21) | (1 << CS20);
	
	//trigger the interrupt vector TIMER1_OVF_vect when timer 1 is overflow
	TIMSK = (1 << TOV2);
	
	//sets the interruptions to enabled
	sei();
}


int main(void)
{
	DDRD = 0xFF;
	DDRE = 0xFF;
	DDRA = 0xFF;
	DDRC = 0xFF;
	
	//A0-A3 : are the negative row 1-4
	//C4-C7 : are the negative row 5-8
	
	XDIV = 0x00;

	init_timer2_OVF();
		
	
	while (1){
	
		showMatrix();
				
	}
	
}