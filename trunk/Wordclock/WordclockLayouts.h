#include "Arduino.h"
#include "WordclockLeds.h"

#ifndef __WordclockLayouts_h__
#define __WordclockLayouts_h__




//Draws the time in the matrix. Here you implement YOUR version of the design.
//Expects second,minute,hour,null,day,month,year
//This version is the "BCD v1" layout
void MapTimeInLedMatrix_BCD1(int pTimeArray[]){
   resetLedMatrix();

   /*
   BCD : Binary coded decimal for the geeks among hte geeks ... 
   I'm not in this group but making it is a nice POC.
    xx xx xx xx xx
    xx h3 xx xx m3
    xx h2 xx mm m2
    hh h1 xx mm m1 
    hh h0 xx mm m0
    */

   int vH = pTimeArray[2];
   //high part of hour
   setCell(3, 0, vH >= 20);
   setCell(4, 0, vH >= 10 && vH < 20);
   //low part of hour
   vH = vH % 10;
   setCell(1, 1, (vH & 0x08) != 0);
   setCell(2, 1, (vH & 0x04) != 0);
   setCell(3, 1, (vH & 0x02) != 0);
   setCell(4, 1, (vH & 0x01) != 0);
   


   int vM = pTimeArray[1] / 10;
   //high part of minutes
   setCell(2, 3, (vM & 0x04) != 0);
   setCell(3, 3, (vM & 0x02) != 0);
   setCell(4, 3, (vM & 0x01) != 0);
   //low part of minutes
   vM = pTimeArray[1] % 10;
   setCell(1, 4, (vM & 0x08) != 0);
   setCell(2, 4, (vM & 0x04) != 0);
   setCell(3, 4, (vM & 0x02) != 0);
   setCell(4, 4, (vM & 0x01) != 0);
}



//Draws the time in the matrix. Here you implement YOUR version of the design.
//Expects second,minute,hour,null,day,month,year
//This version is the "original v1" layout
void MapTimeInLedMatrix_TheOriginal(int pTimeArray[]){
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


#endif //__WordclockLayouts_h__
