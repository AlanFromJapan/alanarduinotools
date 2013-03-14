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


#define MODE_COUNT 3
#define MODE_NEXUS 2
#define MODE_DIGITS 1
#define MODE_SLIDE 0
volatile uint8_t mShowMode = MODE_SLIDE;






ISR(TIMER2_OVF_vect){
	//RandomColors();
	//Waves1();
	//WavesRandom();

	//if (mShowMode == MODE_NEXUS){
		//NexusLike();
	//}
	//else {
		//if (mTiming == 400){
			//mTiming = 0;
			//mCount = (mCount >= 9 ? 0 : mCount+1);
		//
			//uint8_t vColor = rand() % 3;
			//vColor = idToRGB(vColor);
//
		//
			//ShowOne(vColor, mCount);
		//
			////matrixSlide(-1);
		//}
		//mTiming++;
	//}	
	
	switch(mShowMode){
		case MODE_NEXUS:
			NexusLike();
			break;
		case MODE_DIGITS:
			if (mTiming == 400){
				mTiming = 0;
				mCount = (mCount >= 9 ? 0 : mCount+1);
	
				uint8_t vColor = rand() % 3;
				vColor = idToRGB(vColor);
	
	
				ShowOne(vColor, mCount);
	
				//matrixSlide(-1);
			}
			mTiming++;
		break;
		case MODE_SLIDE:
			if (mTiming == 200){
				mTiming = 0;
				mCount = (mCount >= 8+DIGIT_WIDTH*5-1 ? 0 : mCount+1);
				
				ShowDigits(6789, mCount);
			}
			mTiming++;
		
		break;		
	}

			
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
	//PORTS A,C,E,D are dedicated to matrix display
	DDRD = 0xFF;
	DDRE = 0xFF;
	DDRA = 0xFF;
	DDRC = 0xFF;
	
	//G port is for input
	DDRG = 0x00;
	//pullups for everyone
	PORTG = 0xff;
	//just make sure pullups are NOT disabled
	MCUCR |= (0 << PUD);
	
	//A0-A3 : are the negative row 1-4
	//C4-C7 : are the negative row 5-8
	
	XDIV = 0x00;

	init_timer2_OVF();
		
	while (1){
	
		showMatrix();
		
		//check if PING3 is pressed (back board button)
		if ((~PING & (1 << PING3)) != 0){
			matrixClearAll();
			mShowMode = (mShowMode + 1) % MODE_COUNT;
		}
	}
	
}