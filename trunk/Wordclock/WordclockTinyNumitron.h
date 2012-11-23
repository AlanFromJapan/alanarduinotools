#include "Arduino.h"

#ifndef __WordclockTinyNumitron_h__
#define __WordclockTinyNumitron_h__

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


void setupTinyNumitron7seg() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void drawLedMatrix_TinyNumitron7seg() {
   //basically override the "default version". do nothing for some time and the refresh
   delay(3000);
}

//Expects second,minute,hour,null,day,month,year
//This version is the "Tiny Numitron IV16 x4 hh mm" layout
void MapTimeInLedMatrix_TinyNumitronIV16(int pTimeArray[]){
   
    digitalWrite(latchPin, LOW);
    
   //Minutes
   //units
   shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[pTimeArray[1] % 10]);
   //tens
   shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[pTimeArray[1] / 10]);

   //Hours
   //units
   shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[pTimeArray[2] % 10]);
   //tens
   //if less than 10 turn the tens digit off for hours
   if (pTimeArray[2] < 10){
      shiftOut(dataPin, clockPin, MSBFIRST, DIGIT_OFF);
   }
   else {
      shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[pTimeArray[2] / 10]);
   }
   
   
    digitalWrite(latchPin, HIGH);
}

void loop9999(){
   boolean vLight = false;
   
   for (int i = 0; i < 10000; i++){
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    //units
    shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[i % 10]             | ( vLight && (i / 10) % 4 == 0 ? 0b10000000 : 0x00));  
    
    // shift out the bits:
    //tens
    shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[(i / 10) % 10]      | ( vLight && (i / 10) % 4 == 1 ? 0b10000000 : 0x00));  
    //hundreds
    shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[(i / 100) % 10]     | ( vLight && (i / 10) % 4 == 2 ? 0b10000000 : 0x00));  
    //thousands
    shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[(i / 1000) % 10]    | ( vLight && (i / 10) % 4 == 3 ? 0b10000000 : 0x00));   

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(1000);
    
   }
}

#endif //__WordclockTinyNumitron_h__


