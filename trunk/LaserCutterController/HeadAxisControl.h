#include "Arduino.h"

#ifndef __HeadAxiControl_h__
#define __HeadAxiControl_h__

#include "HeadGlobals.h"

//#define USE_HEAD_ADJUSTMENT


//is the head on the stopper ? (read the pin directly)
#define isHeaderStopper() ((PIND & 0x04) == 0x00)


void setupHeadPositionControl(){

  DDRC &= 0xFE;     // Clear the PC0 and c1 pin
  // PC0 (PCINT0 pin) is now an input
  
  PORTC |= 0x01;    // turn On the Pull-up
    
  PCICR = (1 << PCIE1);    // set PCIE0 to enable PCMSK0 scan
  PCMSK1 = (1 << PCINT8)  ;  // set PCINT0 to trigger an interrupt on state change 

}



byte toggleHeadDirection (){
  //first stop
  stopHead();

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



void moveHeadByAmount (int pDistance, uint8_t pSpeed){
  setHeadSpeed(pSpeed);
  
#ifdef USE_SERIAL  
  Serial.print("start moveByAmount(");Serial.print(pDistance);Serial.print(");POS=");Serial.println(mHeadPos);
#endif //USE_SERIAL

  //we're already at the leftmost position, can't go further
  if ((isHeaderStopper()) && pDistance < 0){
    mHeadPos = 0;
    
#ifdef USE_SERIAL
    Serial.print("  Already leftmost, cancelled movement ! POS=");Serial.println(mHeadPos);
#endif //USE_SERIAL  

    return;
  }

  //we're already at the rightmost position, can't go further
  if (mHeadPos >= HEAD_MAX_DISTANCE && pDistance > 0){
#ifdef USE_SERIAL
    Serial.print("  Already rightmost, cancelled movement ! POS=");Serial.println(mHeadPos);
#endif //USE_SERIAL  

    return;
  }
  
  mHeadLeftRight = (pDistance < 0 ? true : false);
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
  analogWrite(vPin, mHeadSpeed);       
  
  //move
  do {
//    delay(1);
    vCurrentDistance = abs(vStartPos - mHeadPos);
    
//    Serial.print("  moving ... POS=");Serial.println(mHeadPos);
    
  } while (
    !(mHeadLeftRight && isHeaderStopper())  
    && vCurrentDistance < vTargetDistance 
    && !(!mHeadLeftRight && mHeadPos >= HEAD_MAX_DISTANCE)
    );
  
#ifdef USE_HEAD_ADJUSTMENT  
  if (vCurrentDistance > vTargetDistance && !isHeaderStopper() && mHeadPos <= HEAD_MAX_DISTANCE) {
#ifdef USE_SERIAL
    Serial.print("  Too far ! POS=");Serial.println(mHeadPos);
#endif //USE_SERIAL

    //in case we moved too far, get back just a little
    vPin = toggleHeadDirection();
    //change speed
    analogWrite(vPin, PWMSPEED_ADJUST); 
    while (vCurrentDistance > vTargetDistance && !isHeaderStopper() && mHeadPos <= HEAD_MAX_DISTANCE) {
//      delay(1);
      vCurrentDistance = abs(vStartPos - mHeadPos);

      if (isHeaderStopper()) break;
    }
  }
#endif //USE_HEAD_ADJUSTMENT

  //stop
  stopHead();      

#ifdef USE_SERIAL
  Serial.print("finish moveByAmount(");Serial.print(pDistance);Serial.print(");POS=");Serial.println(mHeadPos);
#endif //USE_SERIAL  
}


void moveHeadToPosition (int pTargetPosition){
  moveHeadByAmount (pTargetPosition - mHeadPos, mHeadSpeed);
}

void setHeadLeftmost(){
  //reset position to far right so we'll go max left, hit the stopper, know where is the 0
  mHeadPos = HEAD_MAX_DISTANCE;
  
  setHeadSpeed(PWMSPEED_FAST);
  moveHeadByAmount(-HEAD_MAX_DISTANCE -1000, PWMSPEED_FAST);
  setHeadSpeed(PWMSPEED);
}

void setupHeadStopperInterrupt(){
  //D2 is input
  DDRD &= ~(1 << 2);
  //pullup on D2
  PORTD |= (1 << 2);
  
  //interrupt on falling hedge of INT0
  EICRA |= (1 << ISC01);
  //INT0 enabled
  EIMSK |= (1 << INT0);
}



//Handle the pin status change on the ADC0
ISR (PCINT1_vect)
{
    if (mHeadLeftRight)
      mHeadPos -=1;
    else 
      mHeadPos +=1;
}

//handler for INT0 : head hits leftmost stopper
ISR (INT0_vect) {

  //ignore if going right
  if (!mHeadLeftRight)
    return;
    
  stopHead();
  mHeadPos = 0;

#ifdef USE_SERIAL  
  Serial.println("Stoooooooop ! We hit the stopper.");
#endif //USE_SERIAL  
}

#endif //__HeadAxiControl_h__
