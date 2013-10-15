#include "Arduino.h"

#ifndef __Joystick_h__
#define __Joystick_h__

#define JOYSTICK_AVG8BITS   128
#define JOYSTICK_RNG8BITS   32

#define PIN_JOYSTICK_X 1
#define PIN_JOYSTICK_Y 2

//returns -1,0,1 for x and y
void readJoystick (int8_t* x, int8_t* y){
  int vValue = analogRead(PIN_JOYSTICK_X);
  vValue >>= 2;
  if (vValue <= (JOYSTICK_AVG8BITS - JOYSTICK_RNG8BITS)){
    *x = (int8_t)-1;
  }
  else if (vValue >= (JOYSTICK_AVG8BITS + JOYSTICK_RNG8BITS)){
    *x = (int8_t)1;
  }
  else {
    *x = (int8_t)0;
  }
  
  
  vValue = analogRead(PIN_JOYSTICK_Y);
  vValue >>= 2;
  if (vValue <= (JOYSTICK_AVG8BITS - JOYSTICK_RNG8BITS)){
    *y = (int8_t)1;
  }
  else if (vValue >= (JOYSTICK_AVG8BITS + JOYSTICK_RNG8BITS)){
    *y = (int8_t)-1;
  }
  else {
    *y = (int8_t)0;
  }
}

/*
void testJoystick(){
  Serial.println("aaaa");
  while (1) {
    int8_t x, y;
    readJoystick (&x, &y);
    Serial.print("x=");Serial.print(x);Serial.print(";y=");Serial.print(y);Serial.println("");
    
    delay(250);
  }
  Serial.println("bbbb");
}
*/

#endif //__Joystick_h__
