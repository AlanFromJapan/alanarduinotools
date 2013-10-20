#include "Arduino.h"

#ifndef __BedControl_h__
#define __BedControl_h__

#include "BedGlobals.h"

void setupBed() {  
  pinMode(BED_STOPPER_PIN, INPUT);
  
  pinMode(COILA_PIN1, OUTPUT);
  pinMode(COILA_PIN2, OUTPUT);
  pinMode(COILB_PIN1, OUTPUT);
  pinMode(COILB_PIN2, OUTPUT);

  digitalWrite(COILA_PIN1, LOW);
  digitalWrite(COILA_PIN2, LOW);
  digitalWrite(COILB_PIN1, LOW);
  digitalWrite(COILB_PIN2, LOW);
  
}

//set the speed of the bed motor (in fact the on/off time of the coil)
void setBedSpeed (int pSpeed){
  mBedCurrentSpeed = pSpeed;
}


//moving the bed stepper coils
void moveCoil (int pCoilBasePin, boolean lowFirst){
  digitalWrite(pCoilBasePin, (!lowFirst ? LOW : HIGH));
  digitalWrite(pCoilBasePin+1, (lowFirst ? LOW : HIGH));
  delay(mBedCurrentSpeed);
  digitalWrite(pCoilBasePin, LOW);
  digitalWrite(pCoilBasePin+1, LOW);
}


//move the bed toward South
void moveBackward () {
  moveCoil(COILA_PIN1, false);
  moveCoil(COILB_PIN1, true);
  moveCoil(COILA_PIN1, true);
  moveCoil(COILB_PIN1, false);
}

//move the bed toward North
void moveForward () {
  moveCoil(COILA_PIN1, true);
  moveCoil(COILB_PIN1, true);
  moveCoil(COILA_PIN1, false);
  moveCoil(COILB_PIN1, false);
}


//find the stepper spot (for initialization)
void resetBedToStopper(){
  setBedSpeed(DELAY_COIL_MS_FAST);  

  if (digitalRead(BED_STOPPER_PIN)){
    //it's already blocked so go south
    while (digitalRead(BED_STOPPER_PIN)){
      moveBackward ();    
    }
  }
  else {
    //it's not blocked so go north
    while (!digitalRead(BED_STOPPER_PIN)){
      moveForward ();    
    }  
  }
  
  setBedSpeed(DELAY_COIL_MS);
  
  mBedPos = 0;
}


void moveBedNorth(int pSteps){
  for (; pSteps > 0 && mBedPos <= MAX_STEPS_NORTH ; pSteps--){
    moveForward();
    mBedPos++;
  }
  
#ifdef USE_SERIAL
  Serial.print("BED POS=");Serial.println(mBedPos);
#endif //USE_SERIAL
}


void moveBedSouth(int pSteps){
  for (; pSteps > 0 ; pSteps--){
    moveBackward();
    mBedPos--;
  }

#ifdef USE_SERIAL
  Serial.print("BED POS=");Serial.println(mBedPos);
#endif //USE_SERIAL
}

void moveBedToPosition (int pPosition){
  if (pPosition - mBedPos > 0) {
    moveBedNorth(abs(pPosition - mBedPos));
  }
  else{
    moveBedSouth(abs(pPosition - mBedPos));    
  }
}

#endif //__BedControl_h__
