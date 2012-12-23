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


#endif //__WordclockShared_h__
