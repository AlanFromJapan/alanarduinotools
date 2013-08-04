#include "Arduino.h"

#ifndef __WordclockShared_h__
#define __WordclockShared_h__

typedef struct {
  byte second;
  byte minute;
  byte hour;
  byte dayOfWeek;
  byte dayOfMonth; 
  byte month;
  byte year;
} 
Date;


// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}

//alternate way to do it
byte bcd2Dec (byte val){
  byte v  = val & 0x0f;
  
  if ((val  & (1 << 4)) != 0){
    v += 10;
  }
  if ((val  & (1 << 5)) != 0){
    v += 20;
  }
  if ((val  & (1 << 6)) != 0){
    v += 40;
  }
  
  return v;
}
#endif //__WordclockShared_h__
