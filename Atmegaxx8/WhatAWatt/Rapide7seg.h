/*
 * Rapide7seg.h
 *
 * Created: 2012/12/11 23:14:02
 *  Author: Alan
 */ 


#include <avr/io.h>
#include <util/delay.h>

#ifndef RAPIDE7SEG_H_
#define RAPIDE7SEG_H_


#define DIGIT_OFF	0b11111111
#define DIGIT_DOT	0b01111111
#define DIGIT_0		0b11000000
#define DIGIT_1		0b11111001
#define DIGIT_2		0b10100100
#define DIGIT_3		0b10110000
#define DIGIT_4		0b10011001
#define DIGIT_5		0b10010010
#define DIGIT_6		0b10000010
#define DIGIT_7		0b11111000
#define DIGIT_8		0b10000000
#define DIGIT_9		0b10010000
#define DIGIT_A		0b10001000
#define DIGIT_B		0b10000011
#define DIGIT_C		0b11000110
#define DIGIT_D		0b10100001
#define DIGIT_E		0b10000110
#define DIGIT_F		0b10001110

uint8_t DIGITS[] = {
	DIGIT_0,
	DIGIT_1,
	DIGIT_2,
	DIGIT_3,
	DIGIT_4,
	DIGIT_5,
	DIGIT_6,
	DIGIT_7,
	DIGIT_8,
	DIGIT_9,
	DIGIT_A,
	DIGIT_B,
	DIGIT_C,
	DIGIT_D,
	DIGIT_E,
	DIGIT_F
};

//one value, one digit, left to right
uint8_t mDisplayTab[4];


#define POV_DURATION_US 500
//Shows the in-memory table of 4 7segs
//Says from which to start and till which to go (0= leftmost, 3= rightmost)
void showDisplayTab(uint8_t pFromLeft, uint8_t pToRight){
	if (pFromLeft <= 0 && pToRight >= 0){
		PORTD = mDisplayTab[0];
		PORTC = 0x01;
		_delay_us(POV_DURATION_US);
	}

	if (pFromLeft <= 1 && pToRight >= 1){
		PORTD = mDisplayTab[1];
		PORTC = 0x02;
		_delay_us(POV_DURATION_US);
	}
	
	if (pFromLeft <= 2 && pToRight >= 2){
		PORTD = mDisplayTab[2];
		PORTC = 0x04;
		_delay_us(POV_DURATION_US);
	}

	if (pFromLeft <= 3 && pToRight >= 3){
		PORTD = mDisplayTab[3];
		PORTC = 0x08;
		_delay_us(POV_DURATION_US);
	}
}

#define POV_ITERATIONS 25
void showNumber(uint16_t pNumber, uint8_t pFromLeft, uint8_t pToRight, bool pShowLeadingZeros){
	//display on the 3 leftmost digits
	mDisplayTab[0] = DIGITS[(pNumber / (uint16_t)1000) % (uint16_t)10];
	if (!pShowLeadingZeros && mDisplayTab[0] == DIGIT_0){
		mDisplayTab[0] = DIGIT_OFF;
	}
	
	mDisplayTab[1] = DIGITS[(pNumber / (uint16_t)100) % (uint16_t)10];
	if (!pShowLeadingZeros && mDisplayTab[0] == DIGIT_OFF && mDisplayTab[1] == DIGIT_0){
		mDisplayTab[1] = DIGIT_OFF;
	}			
	
	mDisplayTab[2] = DIGITS[(pNumber / (uint16_t)10) % (uint16_t)10];		
	mDisplayTab[3] = DIGITS[pNumber % (uint16_t)10];
	
	for (uint16_t vPOV = 0; vPOV < POV_ITERATIONS; vPOV++){
		showDisplayTab(pFromLeft,pToRight);
	}
}

void setup7seg(){
		
	//Driver of the common anode is from PC0 (1st 7seg) to PC3 (4th 7seg)
	//Lower 4 go output
	DDRC = 0x0F;
	//all segments on
	PORTC = 0xFF;
		
	//the 7seg legs are driven by pins PIND
	//all pins are output
	DDRD = 0xFF;
}


#endif /* RAPIDE7SEG_H_ */