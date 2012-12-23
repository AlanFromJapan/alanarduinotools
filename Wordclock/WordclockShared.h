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


//second,minute,hour,null,day,month,year
Date dateArrayToDate (int* pTimeArray, Date& pDate){
  pDate.second = pTimeArray[0];
  pDate.minute = pTimeArray[1];
  pDate.hour = pTimeArray[2];
  //pDate.second = pTimeArray[3];
  pDate.dayOfMonth = pTimeArray[4];
  pDate.month = pTimeArray[5];
  pDate.year = pTimeArray[6];
  
  return pDate;
}

//second,minute,hour,null,day,month,year
int* dateDateToArray (int* pTimeArray, Date& pDate){
   pTimeArray[0] = pDate.second;
   pTimeArray[1] = pDate.minute;
   pTimeArray[2] = pDate.hour;
   //pTimeArray[3] = null;
   pTimeArray[4] = pDate.dayOfMonth;
   pTimeArray[5] = pDate.month;
   pTimeArray[6] = pDate.year;
   
   return pTimeArray;
}

#endif //__WordclockShared_h__
