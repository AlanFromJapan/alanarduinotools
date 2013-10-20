#include "Arduino.h"

#ifndef __HeadGlobals_h__
#define __HeadGlobals_h__

#define PWM_PIN_LEFT  9
#define PWM_PIN_RIGHT 3

#define PWMSPEED          45
#define PWMSPEED_FAST     60
#define PWMSPEED_ADJUST   30

#define HEAD_MAX_DISTANCE 4000

volatile uint8_t mHeadSpeed = PWMSPEED_FAST;

//how many steps to do a 1mm
#define MILLI_HEAD_STEPS  14
#define CENTI_HEAD_STEPS  143


//position of the head
volatile int mHeadPos = 0;
//going left ?
volatile boolean mHeadLeftRight = true;

inline void stopHead() {
  //first stop
  analogWrite(PWM_PIN_LEFT, 0);
  analogWrite(PWM_PIN_RIGHT, 0);
}

inline void setHeadSpeed(uint8_t pSpeed) {
  mHeadSpeed = pSpeed;
}  
#endif //__HeadGlobals_h__
