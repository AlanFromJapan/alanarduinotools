#include "DS3234.h"
#include "WordclockLeds.h"
#include "WordclockLayouts.h"

//DS3234.h requires this to be in the main .ino file.
#include "SPI.h"

void setup() {
   setupLedMatrix();
   RTC_init(false);
   
   /*   
   //day(1-31), month(1-12), year(0-99), hour(0-23), minute(0-59), second(0-59)
   SetTimeDate(
   01,02,12, //dd,MM,yy
   23,34,00  //hh,mm,ss
   ); 
   */
}

void loop() {

   //second,minute,hour,null,day,month,year
   int vTimeArray[7];
   ReadTimeArray_Fake(&vTimeArray[0], 10);
   //Draw the in-memory matrix
   MapTimeInLedMatrix_BCD1(vTimeArray);
   //Draw the matrix in memory on the leds
   drawLedMatrix(); 
   //a little rest
   delay(5);
}


//Reads FAKE time (time passes fast, for test purpose)
//second,minute,hour,null,day,month,year	
void ReadTimeArray_Fake(int* TimeDate, int SpeedFactor){
   //(1/SpeedFactor) sec is one minute
   unsigned long vTime = ((millis()* (unsigned long)SpeedFactor) / ((unsigned long)1000)) % (unsigned long)(24 * 60);
   
   *TimeDate = 0;
   *(TimeDate + 1) = (int)(vTime % 60);
   *(TimeDate + 2) = (int)(vTime / 60);
   //ignore the rest...
}





