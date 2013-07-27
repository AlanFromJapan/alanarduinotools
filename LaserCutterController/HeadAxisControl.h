#include "Arduino.h"

#ifndef __HeadAxiControl_h__
#define __HeadAxiControl_h__

#define USE_SERIAL

#define PWM_PIN_LEFT  3
#define PWM_PIN_RIGHT 9

#define PMWSPEED          50
#define PMWSPEED_ADJUST   20
#define PWMSTEP           30
#define PWMSTEPSMALL      5

volatile int mHeadPos = 0;
volatile boolean mHeadLeftRight = true;

void setupPositionControl(){

  DDRC &= 0xFC;     // Clear the PC0 and c1 pin
  // PB0 (PCINT0 pin) is now an input
  
  PORTC |= 0x03;    // turn On the Pull-up
  // PB0 is now an input with pull-up enabled
  
  
  PCICR = (1 << PCIE1);    // set PCIE0 to enable PCMSK0 scan
  PCMSK1 = (1 << PCINT8) | (1 << PCINT9) ;  // set PCINT0 to trigger an interrupt on state change 

}



byte toggleDirection (){
  //first stop
  analogWrite(PWM_PIN_LEFT, 0);
  analogWrite(PWM_PIN_RIGHT, 0);

  //reverse
  mHeadLeftRight = !mHeadLeftRight;

  //return the right PIN
  if (mHeadLeftRight){
    return PWM_PIN_LEFT;
  }
  else {
    return PWM_PIN_RIGHT;    
  }  
}

void moveByAmount (int pDistance){
#ifdef USE_SERIAL  
  Serial.print("start moveByAmount(");Serial.print(pDistance);Serial.print(");POS=");Serial.println(mHeadPos);
#endif //USE_SERIAL

  mHeadLeftRight = (pDistance > 0 ? true : false);
  unsigned int vCurrentDistance = 0;
  unsigned int vTargetDistance = abs(pDistance);
  int vStartPos = mHeadPos;
  byte vPin = 0;
  
  // change the analog out value:
  if (mHeadLeftRight){
    vPin = PWM_PIN_LEFT;
  }
  else {
    vPin = PWM_PIN_RIGHT;    
  }
  //start moving
  analogWrite(vPin, PMWSPEED);       
  
  //move
  while (vCurrentDistance < vTargetDistance) {
    delay(PWMSTEPSMALL);
    vCurrentDistance = abs(vStartPos - mHeadPos);

    //Serial.print("vCurrentDistance");Serial.println(mHeadPos);
  }
  
  if (vCurrentDistance > vTargetDistance) {
#ifdef USE_SERIAL
    Serial.print("  Too far ! POS=");Serial.println(mHeadPos);
#endif //USE_SERIAL

    //in case we moved too far, get back just a little
    vPin = toggleDirection();
    //change speed
    analogWrite(vPin, PMWSPEED_ADJUST); 
    while (vCurrentDistance > vTargetDistance) {
      delay(1);
      vCurrentDistance = abs(vStartPos - mHeadPos);

      //Serial.print("  Too far ! POS=");Serial.println(mHeadPos);
    }
  }
  
  //stop
  analogWrite(vPin, 0);       

#ifdef USE_SERIAL
  Serial.print("finish moveByAmount(");Serial.print(pDistance);Serial.print(");POS=");Serial.println(mHeadPos);
#endif //USE_SERIAL  
}


void moveToPosition (int pTargetPosition){
  moveByAmount (pTargetPosition - mHeadPos);
}

//Handle the pin status change on the ADC0-1
ISR (PCINT1_vect)
{
    if (mHeadLeftRight)
      mHeadPos +=1;
    else 
      mHeadPos -=1;
}

#endif //__HeadAxiControl_h__
