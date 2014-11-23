
#ifndef __MCMShared_h__
#define __MCMShared_h__

// --------- Constants taken from Arduino.h ----------
#define HIGH 0x1
#define LOW  0x0

#define true 0x1
#define false 0x0

#define LSBFIRST 0
#define MSBFIRST 1
// --------- /Constants taken from Arduino.h ----------


typedef struct {
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
  uint8_t dayOfWeek;
  uint8_t dayOfMonth; 
  uint8_t month;
  uint8_t year;
} 
Date;


// Convert normal decimal numbers to binary coded decimal
uint8_t decToBcd(uint8_t val)
{
  return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
uint8_t bcdToDec(uint8_t val)
{
  return ( (val/16*10) + (val%16) );
}

//alternate way to do it
uint8_t bcd2Dec (uint8_t val){
  uint8_t v  = val & 0x0f;
  
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
#endif //__MCMShared_h__
