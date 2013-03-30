/*
 * Font.h
 *
 * Created: 2013/03/12 21:13:48
 *  Author: Alan
 */ 


#ifndef FONT_H_
#define FONT_H_

#define DIGIT_HEIGHT 6
#define DIGIT_WIDTH  5


uint8_t DIGIT0[] = {
	0b01100,
	0b10010,
	0b10010,
	0b10010,
	0b10010,
	0b01100
	};
	
uint8_t DIGIT1[] = {
	0b00100,
	0b00110,
	0b00100,
	0b00100,
	0b00100,
	0b01110
};

uint8_t DIGIT2[] = {
	0b01110,
	0b10000,
	0b01000,
	0b00100,
	0b00010,
	0b11110,
};

uint8_t DIGIT3[] = {
	0b01110,
	0b10000,
	0b01100,
	0b10000,
	0b10000,
	0b01110,
};

uint8_t DIGIT4[] = {
	0b01000,
	0b01100,
	0b01010,
	0b11110,
	0b01000,
	0b01000,
};

uint8_t DIGIT5[] = {
	0b11110,
	0b00010,
	0b01110,
	0b10000,
	0b10000,
	0b01110,
};

uint8_t DIGIT6[] = {
	0b01000,
	0b00100,
	0b00010,
	0b01110,
	0b10010,
	0b01100,
};

uint8_t DIGIT7[] = {
	0b11110,
	0b10000,
	0b01000,
	0b01000,
	0b00100,
	0b00100,
};

uint8_t DIGIT8[] = {
	0b01100,
	0b10010,
	0b01100,
	0b10010,
	0b10010,
	0b01100,
};

uint8_t DIGIT9[] = {
	0b01100,
	0b10010,
	0b11100,
	0b10000,
	0b10000,
	0b01100,
};

uint8_t DIGIT_SEMICOLON[] = {
	0b00000,
	0b00000,
	0b01100,
	0b00000,
	0b01100,
	0b00000,
};

 uint8_t* DIGITS[] ={
	DIGIT0,
	DIGIT1,
	DIGIT2,
	DIGIT3,
	DIGIT4,
	DIGIT5,
	DIGIT6,
	DIGIT7,
	DIGIT8,
	DIGIT9,
	};


void copyToBuffer(uint8_t pRGB, uint8_t pX, uint8_t pSource[]){
	volatile uint8_t* vMx;
	if (pRGB == 'R') {
		vMx = mRedMatrix;
	}
	if (pRGB == 'G') {
		vMx = mGreenMatrix;
	}
	if (pRGB == 'B') {
		vMx = mBlueMatrix;
	}
	
	matrixClearAll();
	
	int8_t i = DIGIT_HEIGHT-1;
	do {
		vMx[i + (8-DIGIT_HEIGHT) / 2] = pSource[i];
		vMx[i + (8-DIGIT_HEIGHT) / 2] <<= pX;
		i--;
	} while (i >= 0);
	
}

void ShowOne (uint8_t pRGB, uint8_t pDigit){
	copyToBuffer (pRGB, 1, DIGITS[pDigit]);
}

/*************************************************************************************************/
/* Sliding digits: draws the piece of digit correctly adjusted in position in the color matrix   */
/*************************************************************************************************/
inline void paintDigit(int8_t pPos, uint8_t pLineData, volatile uint8_t* pMx, int8_t pLineIndex, uint8_t pNthDigit) 
{
	//shift by the left or the right to get the line correctly aligned : will depend on the sign of this variable
	int8_t vShiftAmount = 8 - pPos+DIGIT_WIDTH*pNthDigit;
	if (vShiftAmount < 0){
		vShiftAmount = - vShiftAmount;
		pLineData >>= vShiftAmount;
	}
	else {
		pLineData <<= vShiftAmount;
	}

	//1+pLineIndex because digits are 6 led height and I want to center vertically
	pMx[1+pLineIndex] |= pLineData;
}


/***************************************************************************************/
/* Show sliding digit format 'ab:cd'. Pos is the shifitng amount from initial position */
/***************************************************************************************/
void ShowSlidingDigits (uint16_t pValue, int8_t pPos){
	matrixClearAll();
	
	int8_t i = DIGIT_HEIGHT-1;
	//do while(dec) are best performance loop according atmel optimization doc
	do {
		uint8_t vNthDigit = 0;
		uint8_t vVal = 0;
		uint8_t vLine = 0;

		//This is written as a flatten loop for performance, space vs speed constraint (this must be fast)
		//and writing it as a loop would still require a few ifs-ifs or make it less readable.
		//format ab:cd		
		if (pPos >= 1 + DIGIT_WIDTH * vNthDigit && pPos < 8+DIGIT_WIDTH*(1+vNthDigit)){
			//show digit a
			vVal = (pValue / 1000) % 10;
			vLine = DIGITS[vVal][i];
			
			paintDigit(pPos,vLine,mRedMatrix,i,vNthDigit);
		}
		vNthDigit++;

		
		if (pPos >= 1 + DIGIT_WIDTH * vNthDigit && pPos < 8+DIGIT_WIDTH*(1+vNthDigit)){
			//show digit b
			vVal = (pValue / 100) % 10;
			vLine = DIGITS[vVal][i];
			
			paintDigit(pPos,vLine,mRedMatrix,i,vNthDigit);
		}
		vNthDigit++;



		if (pPos >= 1 + DIGIT_WIDTH * vNthDigit && pPos < 8+DIGIT_WIDTH*(1+vNthDigit)){
			//show semicolon
			vLine = DIGIT_SEMICOLON[i];
	
			paintDigit(pPos,vLine,mBlueMatrix,i,vNthDigit);
			paintDigit(pPos,vLine,mGreenMatrix,i,vNthDigit);
			paintDigit(pPos,vLine,mRedMatrix,i,vNthDigit);
		}
		vNthDigit++;



		if (pPos >= 1 + DIGIT_WIDTH * vNthDigit && pPos < 8+DIGIT_WIDTH*(1+vNthDigit)){
			//show digit c
			vVal = (pValue / 10) % 10;
			vLine = DIGITS[vVal][i];
		
			paintDigit(pPos,vLine,mGreenMatrix,i,vNthDigit);
			paintDigit(pPos,vLine,mRedMatrix,i,vNthDigit);
		}
		vNthDigit++;



		if (pPos >= 1 + DIGIT_WIDTH * vNthDigit && pPos < 8+DIGIT_WIDTH*(1+vNthDigit)){
			//show digit d
			vVal = pValue % 10;
			vLine = DIGITS[vVal][i];
		
			paintDigit(pPos,vLine,mGreenMatrix,i,vNthDigit);
			paintDigit(pPos,vLine,mRedMatrix,i,vNthDigit);
		}
		vNthDigit++;
			
		i--;
	} while (i >= 0);
}

	
#endif /* FONT_H_ */