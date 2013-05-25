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

//precalculated shift bits to save time at exec
uint8_t SHIFTBITS[] = {
   0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

//a faster version of shiftout, will work only with current pinout. In case you want to reuse, make sure you use correct pins.
void shiftOutX (uint8_t pVal){
   //8 = PB0 = data
   //6 = PD6 = clock
   //7 = PD7 = latch



   for (int8_t i = 7; i >= 0; i--)  {
      //digitalWrite(dataPin, !!(val & (1 << (7 - i))));
      if ((pVal & SHIFTBITS[i]) == 0){
         PORTB &= 0b11111110;
      }
      else {
         PORTB |= 0b00000001;         
      }

      //Clock up-down
      //digitalWrite(clockPin, HIGH);
      PORTD |= 0b01000000; // ~PD6

      //just a little time to make sure the registers have time to read
      //asm volatile("nop\n\t"::);
      //-> not needed, just need a few ns according datasheet, at 16MHz the CPU match the requirement even without temporization

      //digitalWrite(clockPin, LOW);  
      PORTD &= 0b10111111; // ~PD6          
   }

 
}
boolean mLightOnOff = false;

void drawLedMatrix_TinyNumitron7seg() {
   //basically override the "default version". do nothing for some time and the refresh
   if (mFastPace){
      delay (max((int)100 - ((int)100 - (int)mAnimationCounter), (int)5));
   }
   else {
      delay(1000);
   }
}

//Expects second,minute,hour,null,day,month,year
//This version is the "Tiny Numitron IV16 x4 hh mm" layout
void MapTimeInLedMatrix_TinyNumitronIV16(Date& pD){

   //every hour and half-hour, animation for 10 seconds   
   if ((pD.minute == 0 || pD.minute == 30) && pD.second < 10) {
      //   if (pTimeArray[0] < 10 || (pTimeArray[0] > 30 && pTimeArray[0] < 40)) {
      if (!mFastPace){
         mFastPace = true;
         mAnimationCounter = 0;
         mLightOnOff = true;
      }
   }  
   else {
      mFastPace = false;
   }

   int vSeconds = mAnimationCounter++;//pTimeArray[0];

   digitalWrite(latchPin, LOW);

   //Minutes
   //units
   shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[pD.minute % 10] | (mFastPace && mLightOnOff ? 0b10000000 : 0x00));
   //tens
   shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[pD.minute / 10] | (mFastPace && mLightOnOff ? 0b10000000 : 0x00));

   //Hours
   //units
   shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[pD.hour % 10] | (mFastPace && mLightOnOff ? 0b10000000 : 0x00));
   //tens
   //if less than 10 turn the tens digit off for hours
   if (pD.hour < 10){
      shiftOut(dataPin, clockPin, MSBFIRST, DIGIT_OFF | (mFastPace  && mLightOnOff? 0b10000000 : 0x00));
   }
   else {
      shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[pD.hour / 10] | (mFastPace && mLightOnOff ? 0b10000000 : 0x00));
   }

   mLightOnOff = !mLightOnOff;
   digitalWrite(latchPin, HIGH);
}

//Expects second,minute,hour,null,day,month,year
//This version is the "Tiny Numitron IV16 x4 hh mm" layout
void MapTimeInLedMatrix_TinyNumitronIV16X(int pTimeArray[]){

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

   //digitalWrite(latchPin, LOW);
   PORTD &= 0b01111111; // ~PD7
   
   //Minutes
   //units
   shiftOutX(DIGITS[pTimeArray[1] % 10] | (mFastPace && vSeconds % 4 == 0? 0b10000000 : 0x00));
   //tens
   shiftOutX(DIGITS[pTimeArray[1] / 10] | (mFastPace && vSeconds % 4 == 1? 0b10000000 : 0x00));

   //Hours
   //units
   shiftOutX(DIGITS[pTimeArray[2] % 10] | (mFastPace && vSeconds % 4 == 2? 0b10000000 : 0x00));
   //tens
   //if less than 10 turn the tens digit off for hours
   if (pTimeArray[2] < 10){
      shiftOutX(DIGIT_OFF | (mFastPace && vSeconds % 4 == 3? 0b10000000 : 0x00));
   }
   else {
      shiftOutX(DIGITS[pTimeArray[2] / 10] | (mFastPace && vSeconds % 4 == 3? 0b10000000 : 0x00));
   }

   //digitalWrite(latchPin, HIGH);
   PORTD |= 0b10000000; // ~PD7  
}

#endif //__WordclockTinyNumitron_h__



