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


volatile uint8_t mCount = 0;
volatile uint8_t mCompo	= 0;
volatile uint8_t mTiming = 0;
volatile uint8_t mLine = 0;
volatile int8_t mLineStart = -1;
volatile int8_t mLineEnd = -1;


void NexusLike() 
{
		if (mTiming >= 10) {
		mTiming = 0;
		volatile uint8_t* vMx;
		volatile uint8_t* vMx2;
		
		if (mCompo == 0){
			vMx = mRedMatrix;
		}
		else {
			if (mCompo == 1){
				vMx = mGreenMatrix;
			}
			else {
				vMx = mBlueMatrix;
			}
		}

		switch(mCompo){
			case 0:
			vMx = mRedMatrix;
			vMx2 = mRedMatrix;
			break;
			case 1:
			vMx = mGreenMatrix;
			vMx2 = mGreenMatrix;
			break;
			case 2:
			vMx = mBlueMatrix;
			vMx2 = mBlueMatrix;
			break;
			case 3:
			vMx = mRedMatrix;
			vMx2 = mGreenMatrix;
			break;
			case 4:
			vMx = mRedMatrix;
			vMx2 = mBlueMatrix;
			break;
			case 5:
			vMx = mGreenMatrix;
			vMx2 = mBlueMatrix;
			break;
		}
		
		if (mLineEnd < 7){
			if (mLineEnd < 0){
				mLine = rand() % 8;
			}
			//new line
			mLineEnd++;

			vMx[mLine] = vMx[mLine] | (1 << mLineEnd);
			vMx2[mLine] = vMx2[mLine] | (1 << mLineEnd);
		}
		else {
			if (mLineStart < 7) {
				mLineStart++;

				vMx[mLine] = vMx[mLine]  & ~(1 << mLineStart);
				vMx2[mLine] = vMx2[mLine]  & ~(1 << mLineStart);
			}
			else {
				vMx[mLine] = 0x00;
				vMx2[mLine] = 0x00;
				mLineStart = -1;
				mLineEnd = -1;
				mCompo = rand()% 6;
			}
		}
		
				
	}
	mTiming++;
}





void WavesRandom() 
{
	if (mTiming >= 2) {
		mTiming = 0;
		
		volatile uint8_t* vMx;
	
		if (mCompo == 0){
			vMx = mRedMatrix;
		}
		else {
			if (mCompo == 1){
				vMx = mGreenMatrix;
			}
			else {
				vMx = mBlueMatrix;
			}
		}

		(vMx)[mCount % 8] = rand();

		if ((mCount % 8) == 7){
			mCompo = rand() % 3;
		}
		
		mCount++;	
	}	
	mTiming++;
}




void RandomColors() 
{
		mCount++;
	
	if (mCount >= 10) {
		mCount = 0;
		mGreenMatrix[rand()%8] = (uint8_t)rand();
		mRedMatrix[rand()%8] = (uint8_t)rand();
		mBlueMatrix[rand()%8] = (uint8_t)rand();
	}	
}






void Waves1()
{
	uint8_t vCount = mCount;
	if (mCount < 64) {
		setMatrix('G', vCount / 8, vCount % 8, 1);
	}
	else {
		vCount -= 64;
		if (mCount < 128) {
			setMatrix('R', vCount / 8, vCount % 8, 1);
		}
		else {
			vCount -= 64;
			if (mCount < 196) {
				setMatrix('B', vCount / 8, vCount % 8, 1);
			}
			
		}
	}
	mCount++;
}



ISR(TIMER2_OVF_vect){
	//RandomColors();
	//Waves1();
	//WavesRandom();

	NexusLike();

	
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