#include "DS3234.h"
#include "WordclockLeds.h"
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
   ReadTimeArray(&vTimeArray[0]);
   //Draw the in-memory matrix
   MapTimeInLedMatrix(vTimeArray);
   //Draw the matrix in memory on the leds
   drawLedMatrix(); 
   //a little rest
   delay(5);
}



//Draws the time in the matrix. Here you implement YOUR version of the design.
//Expects //second,minute,hour,null,day,month,year
void MapTimeInLedMatrix(int pTimeArray[]){
   resetLedMatrix();

   /*
   My matrix : designed to be 5x5
    Top are hours (between []) bottom are minutes
    Since there is no '0' led nor for hours nor for time,
    nothing lit means zero.
    [10 20 01 02 03 
    04 05 06 07 08
    09]10 20 30 40
    50 01 02 03 04
    05 06 07 08 09
    */

   int vH = pTimeArray[2];
   if (vH > 0) {
      setCell(0,0, vH >= 10 && vH < 20);
      setCell(0,1, vH >= 20);
      if (vH % 10 > 0){
         vH = vH % 10 + 1;
         setCell(vH / 5, vH % 5, true);
      }
   }

   int vM = pTimeArray[1];
   if (vM / 10 > 0){ 
      int vMtens = vM / 10 + 11 - 1; //+ 11 shifts you to row 2 col 1 (the 10 minutes) 
      setCell(vMtens / 5, vMtens % 5, true);
   }

   if (vM % 10 > 0 ){
      vM = vM % 10 + 16 -1;
      setCell(vM / 5, vM % 5, true);
   }
}



