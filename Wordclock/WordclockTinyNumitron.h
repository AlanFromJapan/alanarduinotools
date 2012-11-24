#include "Arduino.h"

#ifndef __WordclockTinyNumitron_h__
#define __WordclockTinyNumitron_h__

#ifdef  USE_DISPLAY_NUMITRON
   #define BUTTON_ANALOG_PIN 2
   #define MAP_MATRIX_MFUNC(p) MapTimeInLedMatrix_TinyNumitronIV16(p)
   #define DRAW_MATRIX_FUNC() drawLedMatrix_TinyNumitron7seg()
   #define SETUP_MATRIX() setupTinyNumitron7seg()
#endif

//Pin connected to ST_CP of 74HC595
int latchPin = 7;
//Pin connected to SH_CP of 74HC595
int clockPin = 6;
////Pin connected to DS of 74HC595
int dataPin = 8;

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


uint8_t DIGITS[] = {
   DIGIT0, DIGIT1, DIGIT2, DIGIT3, DIGIT4, 
   DIGIT5, DIGIT6, DIGIT7, DIGIT8, DIGIT9 };


//animation requires fast update
boolean mFastPace = false;
unsigned int mAnimationCounter = 0;

void setupTinyNumitron7seg() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void drawLedMatrix_TinyNumitron7seg() {
   //basically override the "default version". do nothing for some time and the refresh
   if (mFastPace){
      delay (max((int)100 - (int)mAnimationCounter, (int)5));
   }
   else {
      delay(1000);
   }
}

//Expects second,minute,hour,null,day,month,year
//This version is the "Tiny Numitron IV16 x4 hh mm" layout
void MapTimeInLedMatrix_TinyNumitronIV16(int pTimeArray[]){

   //every hour and half-hour, animation for 10 seconds   
   if ((pTimeArray[1] == 0 || pTimeArray[1] == 30) && pTimeArray[0] < 10) {
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
   
   digitalWrite(latchPin, LOW);
    
   //Minutes
   //units
   shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[pTimeArray[1] % 10] | (mFastPace && vSeconds % 4 == 0? 0b10000000 : 0x00));
   //tens
   shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[pTimeArray[1] / 10] | (mFastPace && vSeconds % 4 == 1? 0b10000000 : 0x00));

   //Hours
   //units
   shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[pTimeArray[2] % 10] | (mFastPace && vSeconds % 4 == 2? 0b10000000 : 0x00));
   //tens
   //if less than 10 turn the tens digit off for hours
   if (pTimeArray[2] < 10){
      shiftOut(dataPin, clockPin, MSBFIRST, DIGIT_OFF);
   }
   else {
      shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[pTimeArray[2] / 10] | (mFastPace && vSeconds % 4 == 3? 0b10000000 : 0x00));
   }
   
   
    digitalWrite(latchPin, HIGH);
}


#endif //__WordclockTinyNumitron_h__


