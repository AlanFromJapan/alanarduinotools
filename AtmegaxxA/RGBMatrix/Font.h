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
	0b11111,
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
	0b01100,
	0b00010,
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

void ShowDigits (uint16_t pValue, int8_t pPos){
	volatile uint8_t* vMx;
	vMx = mRedMatrix;

	matrixClearAll();
	
	int8_t i = DIGIT_HEIGHT-1;
	do {
		//vMx[i + (8-DIGIT_HEIGHT) / 2] = pSource[i];
		//vMx[i + (8-DIGIT_HEIGHT) / 2] <<= pX;
		
		if (pPos >= 1 && pPos < 8+DIGIT_WIDTH){
			//show digit 0
			uint8_t vVal = (pPos / 1000) % 10;
			uint8_t vLine = DIGITS[vVal][i];
			
			int8_t vShiftAmount = 8 - pPos;
			if (vShiftAmount < 0){
				vShiftAmount = - vShiftAmount;
				vLine >>= vShiftAmount;
			}
			else {
				vLine <<= vShiftAmount;
			}


			
			
			vMx[i] = vLine;
		}
			
		i--;
	} while (i >= 0);
}

	
#endif /* FONT_H_ */