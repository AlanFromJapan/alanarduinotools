#include "Arduino.h"
#include "HeadAxisControl.h"

#ifndef __TestHead_h__
#define __TestHead_h__

void TestHead_FindBoundaries() {
  //Go left...
  moveHeadByAmount(-1000);
  //reset the position
  mHeadPos=0;
  //...and go all the way right
  moveHeadByAmount(+2000);
  
}

#endif __TestHead_h__
