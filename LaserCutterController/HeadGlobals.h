#include "Arduino.h"

#ifndef __HeadGlobals_h__
#define __HeadGlobals_h__

#define PWM_PIN_LEFT  9
#define PWM_PIN_RIGHT 3

#define PMWSPEED          70
#define PMWSPEED_ADJUST   20

#define HEAD_MAX_DISTANCE 4000

//position of the head
volatile int mHeadPos = 0;
//going left ?
volatile boolean mHeadLeftRight = true;
//is in the stopper position
volatile boolean mHeadStopper = false;

void stopHead() {
  //first stop
  analogWrite(PWM_PIN_LEFT, 0);
  analogWrite(PWM_PIN_RIGHT, 0);
}
#endif //__HeadGlobals_h__
