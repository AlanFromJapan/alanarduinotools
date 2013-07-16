/*
 * SimpleAlgo.h
 *
 * Created: 2013/03/12 21:18:51
 *  Author: Alan
 */ 
#include "Font.h"

#ifndef SIMPLEALGO_H_
#define SIMPLEALGO_H_


volatile uint8_t mCount = 0;
volatile uint8_t mCompo	= 0;
volatile uint16_t mTiming = 0;
volatile uint8_t mLine = 0;
volatile int8_t mLineStart = -1;
volatile int8_t mLineEnd = -1;


volatile uint16_t mTimingDivider = 10;

void NexusLike()
{
	if (mTiming >= mTimingDivider) {
		mTiming = 0;
		
		mTimingDivider = mTimingDivider -1 + (rand()% 3);
		if (mTimingDivider <= 1){
			mTimingDivider = 1;
		}
		
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
	
	if (mCount >= 100) {
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


volatile uint16_t mTime = 0;
volatile uint8_t mCount2 = 0;
volatile uint16_t mTiming2 = 0;
void SlidingTime()
{
	if (mTiming2 == 40){
		mTiming2 = 0;
		mCount2 = (mCount2 >= 8+DIGIT_WIDTH*5-1 ? 0 : mCount2+1);
		
		//value to show, mCount2 is the shift position
		ShowSlidingDigits(mTime, mCount2);
		
		if (mCount2 == 0){
			mTime++;
			if (mTime >= 1000){
				mTime = 0;
			}
		}
	}
	mTiming2++;
}
void SlidingTime(uint16_t pTimeFormatHHmm)
{
	if (mTiming2 == 40){
		mTiming2 = 0;
		mCount2 = (mCount2 >= 8+DIGIT_WIDTH*5-1 ? 0 : mCount2+1);
		
		//value to show, mCount2 is the shift position
		ShowSlidingDigits(pTimeFormatHHmm, mCount2);
	}
	mTiming2++;
}

void threeColors() {
mGreenMatrix[0] =	0b11111111;	
mGreenMatrix[1] =	0b11111111;
mGreenMatrix[2] =	0b11111111;
mGreenMatrix[3] =	0b11111111;
mGreenMatrix[4] =	0b00000000;
mGreenMatrix[5] =	0b00000000;
mGreenMatrix[6] =	0b00000000;
mGreenMatrix[7] =	0b00000000;


mRedMatrix[0] =	0b10000000;
mRedMatrix[1] =	0b11000000;
mRedMatrix[2] =	0b11100000;
mRedMatrix[3] =	0b11110000;
mRedMatrix[4] =	0b11111000;
mRedMatrix[5] =	0b11111100;
mRedMatrix[6] =	0b11111110;
mRedMatrix[7] =	0b11111111;	


mBlueMatrix[0] =	0b00000001;
mBlueMatrix[1] =	0b00000011;
mBlueMatrix[2] =	0b00000111;
mBlueMatrix[3] =	0b00001111;
mBlueMatrix[4] =	0b00011111;
mBlueMatrix[5] =	0b00111111;
mBlueMatrix[6] =	0b01111111;
mBlueMatrix[7] =	0b11111111;
}


#define WORM_BOUNCING		0x00
#define WORM_NOT_BOUNCING	0x01
#define WORM_BODY_LEN 5
struct wormStatus {
	uint8_t headx ;
	uint8_t heady ;
	int8_t deltax ;
	int8_t deltay ;
	uint8_t body[WORM_BODY_LEN];
	uint8_t colorChar;
};
volatile wormStatus mWS;
void wormInit(){
	mWS.headx = 5;
	mWS.heady = 4;
	mWS.deltax = 1;
	mWS.deltay = 0;
	mWS.colorChar = 'G';
	
	for (uint8_t i = 0; i < WORM_BODY_LEN; i++){
		mWS.body[i] = (mWS.headx << 4) | (0x0f & mWS.heady);
	}
}
void wormRandom(uint8_t pBounce) {
	mCount++;

	//temporization	
	if (mCount >= 100) {
		mCount = 0;

		//erase all
		matrixClearAll();
		
		
		//change direction ?		
		if (rand() % 5 == 0){
			mWS.deltax = -1 + (rand() % 3);
			mWS.deltay = -1 + (rand() % 3);
			
		}
		
		//move
		if (pBounce == WORM_BOUNCING){
			//bouncing : change the direction
			uint8_t vBounced = 0;
			
			int8_t vNewx = (int8_t)(mWS.headx) + mWS.deltax;
			if (vNewx > 7 || vNewx < 0){
				mWS.deltax = -mWS.deltax;
				vBounced = 1;
			}

			int8_t vNewy = (int8_t)(mWS.heady) + mWS.deltay;
			if (vNewy > 7 || vNewy < 0){
				mWS.deltay = -mWS.deltay;
				vBounced = 1;
			}
			
			//bounced : change the color
			if (vBounced != 0){
				mWS.colorChar = idToRGB(rand() % POSSIBLE_COLORS);
			}
		}
		else {
			//no bouncing : nothing to do
		}
		
		//new head position
		mWS.headx = (uint8_t)((int8_t)(mWS.headx) + mWS.deltax) % 8;
		mWS.heady = (uint8_t)((int8_t)(mWS.heady) + mWS.deltay) % 8;
					
		//update body info
		for (int8_t i = WORM_BODY_LEN-2 ; i >= 0; i--){
			mWS.body[i+1] = mWS.body[i];
		}
		mWS.body[0] = ((uint8_t)mWS.headx << 4) | (0x0f & (uint8_t)mWS.heady);
		
		//draw all
		for (uint8_t i = 0; i < WORM_BODY_LEN; i++){		
			//mRedMatrix[((uint8_t)mWS.body[i] & 0x0f)]  |= (1 << (uint8_t)(((uint8_t)mWS.body[i]) >> 4));
			setMatrix(mWS.colorChar, (uint8_t)(((uint8_t)mWS.body[i]) >> 4),((uint8_t)mWS.body[i] & 0x0f), 1);
		}		

	}	
}
	
#endif /* SIMPLEALGO_H_ */