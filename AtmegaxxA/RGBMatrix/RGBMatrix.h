/*
 * RGBMatrix.h
 *
 * Created: 2013/03/11 23:01:03
 *  Author: Alan
 */ 


#ifndef RGBMATRIX_H_
#define RGBMATRIX_H_

#include <util/delay.h>
#include <stdlib.h>


#define POV_ON_US 40
#define POV_OFF_US 200


volatile uint8_t mBlueMatrix	[8];
volatile uint8_t mRedMatrix		[8];
volatile uint8_t mGreenMatrix	[8];


void matrixSlide(int8_t pAmt){
	uint8_t vShift = abs(pAmt);
	
	if (pAmt > 0){
		int8_t i = 7;
		do {
			mRedMatrix[i]   = mRedMatrix[i] << vShift;
			mGreenMatrix[i] = mGreenMatrix[i] << vShift;
			mBlueMatrix[i]  = mBlueMatrix[i] << vShift;
			i--;
		} while (i >= 0);		
	}
	else {
		int8_t i = 7;
		do {
			mRedMatrix[i]   = mRedMatrix[i] >> vShift;
			mGreenMatrix[i] = mGreenMatrix[i] >> vShift;
			mBlueMatrix[i]  = mBlueMatrix[i] >> vShift;
			i--;
		} while (i >= 0);
	}
}
	
/************************************************************************/
/* Returns the character corresponding to the int value (0=R, 1=B,...)  */
/************************************************************************/
uint8_t idToRGB( uint8_t pColorIntId )
{
	switch(pColorIntId){
		case 0:
		pColorIntId = 'R';
		break;
		case 1:
		pColorIntId = 'G';
		break;
		case 2:
		pColorIntId = 'B';
		break;
		case 3:
		pColorIntId = 'Y';
		break;
		case 4:
		pColorIntId = 'P';
		break;
		case 5:
		pColorIntId = 'C';
		break;
	}
	return pColorIntId;
}

void matrixClearOne (volatile uint8_t* pMx){
	int8_t i = 7;
	do {
		pMx[i] = 0x00;
		i--;
	} while (i >= 0);
}

void matrixClearAll(){
	matrixClearOne(mRedMatrix);
	matrixClearOne(mGreenMatrix);
	matrixClearOne(mBlueMatrix);
}

void setMatrix(uint8_t pRGB, uint8_t pX, uint8_t pY, uint8_t pValue){
	if (pRGB == 'B') {
		mBlueMatrix[pY] = (pValue == 0 ? mBlueMatrix[pY] & ~(1 << pX) : mBlueMatrix[pY] | (1 << pX));
	}
	if (pRGB == 'R') {
		mRedMatrix[pY] = (pValue == 0 ? mRedMatrix[pY] & ~(1 << pX) : mRedMatrix[pY] | (1 << pX));
	}
	if (pRGB == 'G') {
		mGreenMatrix[pY] = (pValue == 0 ? mGreenMatrix[pY] & ~(1 << pX) : mGreenMatrix[pY] | (1 << pX));
	}
}

void setMatrix2(volatile uint8_t* pMx, uint8_t pX, uint8_t pY, uint8_t pValue){
	pMx[pY] = (pValue == 0 ? pMx[pY] & ~(1 << pX) : pMx[pY] | (1 << pX));
}

void showMatrix(){
	PORTE = 0xff;
	PORTA = 0xf0;
	PORTC = 0x0f;
	
	for (int y = 0; y < 8; y++){
		uint8_t vBlue = mBlueMatrix[y];
		
		//set the line on
		if (y < 4){
			PORTC = (1 << (7-y)) | 0x0f;
			PORTA = 0xf0;
		}
		else {
			//y >= 4
			PORTA = (1 << (7-y)) | 0xf0;
			PORTC = 0x0f;
		}
		

		//blue is simply PORTD
		PORTD = ~vBlue;
		
		_delay_us(POV_ON_US);
		//all off
		PORTD = 0xff;
		_delay_us(POV_OFF_US);
		
	}


	for (int y = 0; y < 8; y++){
		uint8_t vGreen = mGreenMatrix[y];
		
		//set the line on
		if (y < 4){
			PORTC = (1 << (7-y)) | 0x0f;
			PORTA = 0xf0;
		}
		else {
			//y >= 4
			PORTA = (1 << (7-y)) | 0xf0;
			PORTC = 0x0f;
		}
		
		//green is shared on PORTA4-7 & PORTC0-3
		vGreen = ~vGreen; //negative logic
		PORTA = (vGreen << 4) | (PORTA & 0x0f);
		PORTC = (vGreen >> 4) | (PORTC & 0xf0);
		
		_delay_us(POV_ON_US);
		//all off
		PORTA = 0xf0;
		PORTC = 0x0f;
		_delay_us(POV_OFF_US);
		
	}
	
	
	for (int y = 0; y < 8; y++){
		uint8_t vRed = mRedMatrix[y];
		
		//set the line on
		if (y < 4){
			PORTC = (1 << (7-y)) | 0x0f;
			PORTA = 0xf0;
		}
		else {
			//y >= 4
			PORTA = (1 << (7-y)) | 0xf0;
			PORTC = 0x0f;
		}
		
		
		//red is PORTE
		PORTE = ~vRed;
		
		_delay_us(POV_ON_US);
		//all off
		PORTE = 0xff;
		_delay_us(POV_OFF_US);
		
	}
	
	PORTE = 0xff;
	PORTA = 0xf0;
	PORTC = 0x0f;
}


#endif /* RGBMATRIX_H_ */