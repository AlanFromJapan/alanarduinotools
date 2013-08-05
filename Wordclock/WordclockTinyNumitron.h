#include "Arduino.h"

#ifndef __WordclockTinyNumitron_h__
#define __WordclockTinyNumitron_h__

#ifdef  USE_DISPLAY_NUMITRON
	#define MAP_MATRIX_MFUNC(p) 	MapTimeInLedMatrix_TinyNumitronIV16X(p)
	#define DRAW_MATRIX_FUNC() 		drawLedMatrix_TinyNumitron7seg()
	#define SETUP_MATRIX() 			setupTinyNumitron7seg()
	#define SET_EDIT_HOURS() 		numitronSetEdit(1)
	#define SET_EDIT_MINUTES() 		numitronSetEdit(2)
	#define SET_EDIT_FINISH() 		numitronSetEdit(0)
#endif

//Pin connected to ST_CP of 74HC595
#define NUMITRON_PIN_LATCH  7
//Pin connected to SH_CP of 74HC595
#define NUMITRON_PIN_CLOCK  6
////Pin connected to DS of 74HC595
#define NUMITRON_PIN_DATA   8

#define DIGIT0       0b00000100
#define DIGIT1       0b00011111
#define DIGIT2       0b00101000
#define DIGIT3       0b00001001
#define DIGIT4       0b00010011
#define DIGIT5       0b01000001
#define DIGIT6       0b01000000
#define DIGIT7       0b00001111
#define DIGIT8       0b00000000
#define DIGIT9       0b00000001
#define DIGITA       0b00000010
#define DIGITE       0b01100000
#define DIGIT_OFF    0b01111111
#define LED_MASK_ON	 0b10000000
#define LED_MASK_OFF 0b00000000

uint8_t DIGITS[] = {
   DIGIT0, DIGIT1, DIGIT2, DIGIT3, DIGIT4, 
   DIGIT5, DIGIT6, DIGIT7, DIGIT8, DIGIT9 };


//animation requires fast update
boolean mFastPace = false;
uint8_t mAnimationCounter = 0;
uint8_t mNumitronEditStatus = 0x00;

void setupTinyNumitron7seg() {
   //set pins to output so you can control the shift register
   pinMode(NUMITRON_PIN_LATCH, OUTPUT);
   pinMode(NUMITRON_PIN_CLOCK, OUTPUT);
   pinMode(NUMITRON_PIN_DATA, OUTPUT);
}

//precalculated shift bits to save time at exec
uint8_t SHIFTBITS[] = {
   0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

//a faster version of shiftout, will work only with current pinout. In case you want to reuse, make sure you use correct pins.
void shiftOutX (uint8_t pVal){
   for (int8_t i = 7; i >= 0; i--)  {
      if ((pVal & SHIFTBITS[i]) == 0){
         PORTB &= ~0x01;
      }
      else {
         PORTB |= 0x01;         
      }

      //Clock up-down
      //digitalWrite(NUMITRON_PIN_CLOCK, HIGH);
      PORTD |= (1 << NUMITRON_PIN_CLOCK); // ~PD6

      //just a little time to make sure the registers have time to read
      //asm volatile("nop\n\t"::);
      //-> not needed, just need a few ns according datasheet, at 16MHz the CPU match the requirement even without temporization

      //digitalWrite(NUMITRON_PIN_CLOCK, LOW);  
      PORTD &= ~(1 << NUMITRON_PIN_CLOCK); // ~PD6          
   }

 
}

//set the edit mode mask
void numitronSetEdit(uint8_t pMode){
	mNumitronEditStatus = pMode;
}


void drawLedMatrix_TinyNumitron7seg() {
   //basically override the "default version". do nothing for some time and the refresh
   if (mFastPace){
      delay (max((uint8_t)100 - (uint8_t)mAnimationCounter, (uint8_t)5));
   }
   else {
      delay(500);
   }
}


//Expects second,minute,hour,null,day,month,year
//This version is the "Tiny Numitron IV16 x4 hh mm" layout
void MapTimeInLedMatrix_TinyNumitronIV16X(Date& pD){

   //every hour and half-hour, animation for 10 seconds   
   if ((pD.minute == 0 || pD.minute == 30) && pD.second < 7) {
      //   if (pTimeArray[0] < 10 || (pTimeArray[0] > 30 && pTimeArray[0] < 40)) {
      if (!mFastPace){
         mFastPace = true;
         mAnimationCounter = 0;
      }
   }  
   else {
      mFastPace = false;
   }

   int vSeconds = mAnimationCounter++;//pTimeArray[0];

   //digitalWrite(NUMITRON_PIN_LATCH, LOW);
   PORTD &= ~(1 << NUMITRON_PIN_LATCH); // ~PD7
   
   //Minutes
   //units
   shiftOutX(DIGITS[pD.minute % 10] | ((mNumitronEditStatus == 2) || (mFastPace && vSeconds % 4 == 0)? LED_MASK_ON : LED_MASK_OFF));
   //tens
   shiftOutX(DIGITS[pD.minute / 10] | ((mNumitronEditStatus == 2) || (mFastPace && vSeconds % 4 == 1)? LED_MASK_ON : LED_MASK_OFF));

   //Hours
   //units
   shiftOutX(DIGITS[pD.hour % 10] | ((mNumitronEditStatus == 1) || (mFastPace && vSeconds % 4 == 2)? LED_MASK_ON : LED_MASK_OFF));
   //tens
   //if less than 10 turn the tens digit off for hours
   if (pD.hour < 10){
      shiftOutX(DIGIT_OFF | ((mNumitronEditStatus == 1) || (mFastPace && vSeconds % 4 == 3)? LED_MASK_ON : LED_MASK_OFF));
   }
   else {
      shiftOutX(DIGITS[pD.hour / 10] | ((mNumitronEditStatus == 1) || (mFastPace && vSeconds % 4 == 3)? LED_MASK_ON : LED_MASK_OFF));
   }

   //digitalWrite(NUMITRON_PIN_LATCH, HIGH);
   PORTD |= (1 << NUMITRON_PIN_LATCH); // ~PD7  
}

#endif //__WordclockTinyNumitron_h__



