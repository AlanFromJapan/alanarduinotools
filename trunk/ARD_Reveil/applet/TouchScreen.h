#include "WProgram.h"
#include "wiring.h"

#ifndef __TOUCHSCREEN_H__
#define __TOUCHSCREEN_H__

//set debug mode
//#define REVEIL_DEBUG

//modified to match my sparkfun connector
#define xLow  17
#define xHigh 15
#define yLow  16
#define yHigh 14
#define TOUCH_READ_PERIOD 50
#define TIME_READ_TRIGGER (1000 / (TOUCH_READ_PERIOD +1))
byte mReadCount = 0;
boolean mTouching = false;
#define TS_KEY_IN '>'
#define TS_KEY_OUT '<'
#define TS_KEY_PLUS '+'
#define TS_KEY_MINUS '-'


//Returns true if read, and changes values of pX and pY to the touch location
boolean readTouch (int& pX, int& pY){
  pinMode(xLow,OUTPUT);
  pinMode(xHigh,OUTPUT);
  digitalWrite(xLow,LOW);
  digitalWrite(xHigh,HIGH);

  digitalWrite(yLow,LOW);
  digitalWrite(yHigh,LOW);

  pinMode(yLow,INPUT);
  pinMode(yHigh,INPUT);
  delay(10);

  //xLow has analog port -14 !!
  int x=analogRead(yLow -14);

  pinMode(yLow,OUTPUT);
  pinMode(yHigh,OUTPUT);
  digitalWrite(yLow,LOW);
  digitalWrite(yHigh,HIGH);

  digitalWrite(xLow,LOW);
  digitalWrite(xHigh,LOW);

  pinMode(xLow,INPUT);
  pinMode(xHigh,INPUT);
  delay(10);

  //xLow has analog port -14 !!
  int y=analogRead(xLow - 14);

  //returns true in case value is over the predefined min level
  if (x > 5 && y > 5){
    //touch, but to avoid erratic rafal touch, ignore if we were already touching
    if (mTouching){
      return false;
    }
    //otherwise return the position and save the fact we touched
    pX = x;
    pY = y;
    mTouching = true;
    return true;
  }
  mTouching = false;
  return false;
}


//returns which "key" has been "pressed" on the touch screen.
//in fact which zone where the user pressed.
byte readTouchKey(){
  int x,y;
  if (readTouch(x,y)){
#ifdef REVEIL_DEBUG
    Serial.print("Touched: (");
    Serial.print(x,DEC);   
    Serial.print(","); 
    Serial.print(y,DEC); 
    Serial.println(")"); 
#endif     
    //_MY_ screen returns values x = [100;800], y = [100;800]
    if (x < 450){
      if (y < 450) {
        return TS_KEY_IN;
      }
      else {
        return TS_KEY_OUT;
      }
    }
    else {
      if (y < 450) {
        return TS_KEY_PLUS;
      }
      else {
        return TS_KEY_MINUS;
      }  
    } 
  }
  return 0;
}

#endif //__TOUCHSCREEN_H__ 
