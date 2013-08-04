#include "Arduino.h"

#ifndef __WordclockButtons_h__
#define __WordclockButtons_h__

#define BUTTON_ANALOG_PIN 2

#ifdef USE_BUTTONS_ANALOG
  #define BUTTON_STATUS_A readButtonAnalogPressed(1024, 20)
  #define BUTTON_STATUS_B readButtonAnalogPressed(512, 20)
#endif

#ifdef USE_BUTTONS_XTENDED
  #define BUTTON_STATUS_A readButtonXtendedPressed(0)
  #define BUTTON_STATUS_B readButtonXtendedPressed(1)
#endif

void setupButtonsAnalog() {

}

//for the extended board
void setupButtonsXtended() {
  //use buttons on C0 and C1
  //Direction input
  DDRC  &= 0xfc;
  //Pullups
  PORTC |= 0x03;
 
}

boolean readButtonAnalogPressed (int pMidValue, int pTolerance){
   int vVal = analogRead(BUTTON_ANALOG_PIN);

   return vVal <= pMidValue + pTolerance 
      && vVal >= pMidValue - pTolerance;
}

boolean readButtonXtendedPressed (uint8_t pButtonIndex){
  /*
  if (pButtonIndex == 0){
    return (PINC & 0x01) == 0;
  }
  if (pButtonIndex == 1){
    return (PINC & 0x02) == 0;
  }
  */
  return false;
}

#endif //__WordclockButtons_h__
