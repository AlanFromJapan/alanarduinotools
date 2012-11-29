#include "Arduino.h"
#include "WordclockLeds.h"
#include "WordclockShared.h"

#ifndef __WordclockLayouts_h__
#define __WordclockLayouts_h__


#ifdef  USE_DISPLAY_BCD1
   #define BUTTON_ANALOG_PIN 2
   #define MAP_MATRIX_MFUNC(p) MapTimeInLedMatrix_BCD1(p)
   #define DRAW_MATRIX_FUNC() drawLedMatrix()
   #define SETUP_MATRIX() setupLedMatrix()
#endif
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


#ifdef  USE_DISPLAY_THEORIGINAL
   #define BUTTON_ANALOG_PIN 2
   #define MAP_MATRIX_MFUNC(p) MapTimeInLedMatrix_TheOriginal(p)
   #define DRAW_MATRIX_FUNC() drawLedMatrix()
   #define SETUP_MATRIX() setupLedMatrix()
#endif
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


#ifdef  USE_DISPLAY_JAPANESE1
   #define BUTTON_ANALOG_PIN 2
   #define MAP_MATRIX_MFUNC(p) MapTimeInLedMatrix_Japanese1(p)
   #define DRAW_MATRIX_FUNC() drawLedMatrix()
   #define SETUP_MATRIX() setupLedMatrix()
#endif
//Draws the time in the matrix. Here you implement YOUR version of the design.
//Expects second,minute,hour,null,day,month,year
//This version is the "Japanese v1" layout
void MapTimeInLedMatrix_Japanese1(int pTimeArray[]){
   resetLedMatrix();

   /*
   Japanese display [hours]
    [10 01 02 03 04
    05 06 07 08 09]
    02 03 04 05 x10
    01 02 03 04 05
    06 07 08 09 am/PM
    */


   if (pTimeArray[2] == 12) {
      //little exception : I want noon to be displayed as 10+2 of afternoon,
      //not 0 of afternoon. Otherwise algo is great for all the other cases.
      setCell(0,0,true); //10
      setCell(0,2, true); //2
   }
   else {
      int vH = pTimeArray[2] % 12;
      //hour is mod12
      setCell(0, 0, vH >= 10);
      //unit part of hour
      vH = vH % 10;
      if (vH > 0) {
         setCell(vH / 5, vH % 5, true);
      }
   }


   int vM = pTimeArray[1] / 10;
   if (vM >= 1 ){
      //light the "10" char
      setCell(2, 4, true); 
      if (vM >= 2){
         //high part of minutes
         setCell(2, vM -2, true);
      }
   }

   //low part of minutes
   vM = pTimeArray[1] % 10 + 15 -1;
   if ((pTimeArray[1] % 10) != 0) {
      setCell (vM / 5, vM % 5, true);
   }

   //am/pm ?
   setCell(4,4, pTimeArray[2] >= 12);
}


#ifdef  USE_DISPLAY_LAFRANCE1
   #define BUTTON_ANALOG_PIN 2
   #define MAP_MATRIX_MFUNC(p) MapTimeInLedMatrix_LaFrance(p)
   #define DRAW_MATRIX_FUNC() drawLedMatrix2x1()
   #define SETUP_MATRIX() setupLedMatrix2x1()
#endif
//Draws the time in the matrix. Here you implement YOUR version of the design.
//Expects second,minute,hour,null,day,month,year
//This version is the "LaFrance v1" layout, using 2 panels horizontaly
void MapTimeInLedMatrix_LaFrance(int pTimeArray[]){
   resetLedMatrix();

   /*
French display
    <--- hours----> <---minutes--->
    it-is 01 02 03 | 10 11 12 13 15
    quatr 05 06 07 | fortn 16 20 30 
    08 09 10 11 Hs | forty fifty 09
    no-on midnt HH | et 01 02 03 05
    night afternon | quatr 06 07 08
    */

   //Fixed lights: It is
   setCell(0, 0, true); 
   setCell(0,1,true);

   int vHour = pTimeArray[2];
   if (vHour == 12){
      setCell(3, 0, true);
      setCell(3, 1, true);
   }
   else {
      if (vHour == 0 || vHour == 24){
         setCell(3, 2, true);
         setCell(3, 3, true);
      }
      else {
         //ok, so it's a regular hour
         if (vHour >= 13 && vHour <= 17){
            //in French : de l'apres-midi between 1pm and 5pm
            setCell (4, 2, true);
            setCell (4, 3, true);
            setCell (4, 4, true);
         }

         if (vHour >= 18){
            //from 6pm it's a du soir
            setCell(4, 0, true);
            setCell(4, 1, true);
         }

         //move to 12h mode
         vHour = vHour % 12;

         if (vHour == 1){
            //Hour without a 's'
            setCell(3, 4, true);
         }
         else {
            //HourS s'il vous plait
            setCell(2, 4, true);
         }

         //at last, the figure
         if (vHour <= 3){
            setCell(0, 1+vHour, true);
         }
         else {
            if (vHour == 4){
               setCell(1, 0, true);
               setCell(1, 1, true);
            }
            else {
               if (vHour <= 7){
                  setCell (1, 2 + vHour - 5, true);
               }
               else {
                  setCell (2, vHour - 8, true);
               }
            }
         }
      }
   }



   int vMin = pTimeArray[1];
   boolean vMinDone = false;
   //irregulars
   switch (vMin){
   case 10:
      setCell(0, 5, true);
      vMinDone = true;
      break;
   case 11:
      setCell(0, 6, true);
      vMinDone = true;
      break;
   case 12:
      setCell(0, 7, true);
      vMinDone = true;
      break;
   case 13:
      setCell(0, 8, true);
      vMinDone = true;
      break;
   case 14:
      setCell(1, 5, true);
      setCell(1, 6, true);
      vMinDone = true;
      break;
   case 15:
      setCell(0, 9, true);
      vMinDone = true;
      break;
   case 16:
      setCell(1, 7, true);
      vMinDone = true;
      break;
   case 17:
      setCell(0, 5, true);
      setCell(4, 8, true);
      vMinDone = true;
      break;
   case 18:
      setCell(0, 5, true);
      setCell(4, 9, true);
      vMinDone = true;
      break;
   case 19:
      setCell(0, 5, true);
      setCell(2, 9, true);
      vMinDone = true;
      break;
   }

   if (!vMinDone) {
      //the tens
      switch(vMin / 10){
      case 1:
         setCell(0, 5, true);
         break;
      case 2:
         setCell(1, 8, true);
         break;
      case 3:
         setCell(1, 9, true);
         break;
      case 4:
         setCell(2, 5, true);
         setCell(2, 6, true);
         break;
      case 5:
         setCell(2, 7, true);
         setCell(2, 8, true);
         break;
      }

      //and the minutes
      if (vMin == 1){
         //just 'one' not 'and one'
         setCell (3, 6, true);
      }
      else {
         switch (vMin % 10) {
         case 1:
            //just 'one' not 'and one'
            setCell (3, 5, true);
            setCell (3, 6, true);
            break;
         case 2:
            setCell (3, 7, true);
            break;
         case 3:
            setCell (3, 8, true);
            break;
         case 4:
            setCell (4, 5, true);
            setCell (4, 6, true);
            break;
         case 5:
            setCell (3, 9, true);
            break;
         case 6:
            setCell (4, 7, true);
            break;
         case 7:
            setCell (4, 8, true);
            break;
         case 8:
            setCell (4, 9, true);
            break;
         case 9:
            setCell (2, 9, true);
            break;
         }
      }
   }
}


#ifdef  USE_DISPLAY_KOREA3
   #define BUTTON_ANALOG_PIN 2
   #define MAP_MATRIX_MFUNC(p) MapTimeInLedMatrix_Korea3(p)
   #define DRAW_MATRIX_FUNC() drawLedMatrix()
   #define SETUP_MATRIX() setupLedMatrix()
#endif

//Draws the time in the matrix. Here you implement YOUR version of the design.
//Expects second,minute,hour,null,day,month,year
//This version is the "Korea v3", a 1x1 (5 leds x 5 leds) layout
void MapTimeInLedMatrix_Korea3(int pTimeArray[]){
   resetLedMatrix();

   /*
   Korea display [hours]
   Time is rounded to closest 5 minutes
   [10 01 05 03 04
    02 06 65 07 07
    08 08 09 09 HH]
    xx 20 30 50 x10
    xx 40x10 05 mm
    */

   //HOURS HOURS HOURS HOURS HOURS
   //The hours 'si'
   setCell(2, 4, true);
   //hour is 12h based
   int vHour = pTimeArray[2] % 12;
   switch (vHour){
      case 0:
         setCell(0, 0, true);
         setCell(1, 0, true);
         break;
      case 1:
         setCell(0, 1, true);
         break;
      case 2:
         setCell(1, 0, true);
         break;
      case 3:
         setCell(0, 3, true);
         break;      
      case 4:
         setCell(0, 4, true);
         break;
      case 5:
         setCell(0, 2, true);
         setCell(1, 2, true);
         break;
      case 6:
         setCell(1, 1, true);
         setCell(1, 2, true);
         break;
      case 7:
         setCell(1, 3, true);
         setCell(1, 4, true);
         break;
      case 8:
         setCell(2, 0, true);
         setCell(2, 1, true);
         break;
      case 9:
         setCell(2, 2, true);
         setCell(2, 3, true);
         break;
      case 10:
         setCell(0, 0, true);
         break;
      case 11:
         setCell(0, 0, true);
         setCell(0, 1, true);
         break;
   }

   //MINUTES MINUTES MINUTES MINUTES MINUTES 
   //NB: in this version minutes are rounded to closest 5 minutes
   //The minutes 'bun'
   setCell(4, 4, true);
   
   int vMin = pTimeArray[1];
   if ((vMin -3)  % 10 <= 5){
      //if minutes are between x3 and x8, the the '5' minutes
      setCell(4, 3, true);
   }
   
   vMin = vMin /10;
   
   switch(vMin){
      case 1:
         setCell (4, 2, true);
         break;
      case 2:
         setCell (3, 1, true);
         setCell (4, 2, true);
         break;
      case 3:
         setCell (3, 2, true);
         setCell (4, 2, true);
         break;
      case 4:
         setCell (4, 1, true);
         setCell (4, 2, true);
         break;
      case 5:
         setCell (3, 3, true);
         setCell (3, 4, true);
         break;
   }
   
/*
   if (pTimeArray[2] == 12) {
      //little exception : I want noon to be displayed as 10+2 of afternoon,
      //not 0 of afternoon. Otherwise algo is great for all the other cases.
      setCell(0,0,true); //10
      setCell(0,2, true); //2
   }
   else {
      int vH = pTimeArray[2] % 12;
      //hour is mod12
      setCell(0, 0, vH >= 10);
      //unit part of hour
      vH = vH % 10;
      if (vH > 0) {
         setCell(vH / 5, vH % 5, true);
      }
   }


   int vM = pTimeArray[1] / 10;
   if (vM >= 1 ){
      //light the "10" char
      setCell(2, 4, true); 
      if (vM >= 2){
         //high part of minutes
         setCell(2, vM -2, true);
      }
   }

   //low part of minutes
   vM = pTimeArray[1] % 10 + 15 -1;
   if ((pTimeArray[1] % 10) != 0) {
      setCell (vM / 5, vM % 5, true);
   }

   //am/pm ?
   setCell(4,4, pTimeArray[2] >= 12);
   */
}


#ifdef  USE_DISPLAY_WINDMILL
   #define BUTTON_ANALOG_PIN 2
   #define MAP_MATRIX_MFUNC(p) MapTimeInLedMatrix_Windmill(p)
   #define DRAW_MATRIX_FUNC() ;
   #define SETUP_MATRIX() setupLedMatrix()
#endif
int mWindmillPulse = 10;
int mWindmillPulseDelta = 1;
#define WINDMILL_MAXPULSE 500
#define WINDMILL_MINPULSE 200
//Draws the time in the matrix. Here you implement YOUR version of the design.
//Expects second,minute,hour,null,day,month,year
//This version is the "Windmill" layout
void MapTimeInLedMatrix_Windmill(int pTimeArray[]){
   resetLedMatrix();

   /*
   Windmill (5 minutes rounded, center is pulse)
    55 05 06 07 08
    50 04 00 05 09
    45 03 xx 10 10
    40 02 01 15 11
    35 30 25 20 12
    */

   //row, col, value
   //setCell (0, 4, true);

   //Hours
   int vHour = pTimeArray[2] % 12;
   if (vHour == 0) {
      vHour = 12;
   }
   
   if (vHour >= 1) setCell(3,2,true);
   if (vHour >= 2) setCell(3,1,true);
   if (vHour >= 3) setCell(2,1,true);
   if (vHour >= 4) setCell(1,1,true);
   if (vHour >= 5) setCell(0,1,true);
   if (vHour >= 6) setCell(0,2,true);
   if (vHour >= 7) setCell(0,3,true);
   if (vHour >= 8) setCell(0,4,true);
   if (vHour >= 9) setCell(1,4,true);
   if (vHour >= 10) setCell(2,4,true);
   if (vHour >= 11) setCell(3,4,true);
   if (vHour >= 12) setCell(4,4,true);
   
   //Minutes
   int vMinutes = pTimeArray[1] / 5;
   
   if (vMinutes >= 0) setCell(1,2,true);
   if (vMinutes >= 1) setCell(1,3,true);
   if (vMinutes >= 2) setCell(2,3,true);
   if (vMinutes >= 3) setCell(3,3,true);
   if (vMinutes >= 4) setCell(4,3,true);
   if (vMinutes >= 5) setCell(4,2,true);
   if (vMinutes >= 6) setCell(4,1,true);
   if (vMinutes >= 7) setCell(4,0,true);
   if (vMinutes >= 8) setCell(3,0,true);
   if (vMinutes >= 9) setCell(2,0,true);
   if (vMinutes >= 10) setCell(1,0,true);
   if (vMinutes >= 11) setCell(0,0,true);
   
   //center pulse is always on
   setCell(2,2,true);
   
   //Drawing : rewritten here because it's complex (nothing instead of drawLedMatrix())
   //Reason is my white (hours) leds are too bright so I need different POV timing for minutes or hours
   //Any other led would make it just fine ...
      
   //COLUMNS are NEGATIVE side, so turn them off by putting them all HIGH
   //My bad : the pinds are reversed ordered but that was easier to wire...
   for (int i=WCLK_COL1; i> WCLK_COL1 - WCLK_MATRIX_SIZE; i--){
      digitalWrite(i, HIGH);
   } 

   //this does NOT run in constant time because we only turns led on, those off are already off so they are skipped.
   //side effect: the more there is led on, the slower it is to draw it.
   for (int r=0; r < WCLK_MATRIX_SIZE; r++){
      for (int c=0; c < WCLK_MATRIX_SIZE; c++){
         if (0 != (WCLK_LED_MATRIX[r] & (1 << c))){
            //turn the led on
            digitalWrite(r + WCLK_ROWA, HIGH);
            digitalWrite(WCLK_COL1- c, LOW); //minus because reverse order

            //POV
            if (c == 4
               || (r == 0 && c >= 1)
               || (c == 1 && r <= 3)
               || (c == 2 && r == 3)){
               //Hours POV
               delayMicroseconds(10);
            }
            else {
               if (c == 2 && r == 2){
                  //Pulse middle
                  
                  //inspiration from http://thecustomgeek.com/2011/06/17/breathing-sleep-led/
                  if (mWindmillPulse < WINDMILL_MINPULSE) delayMicroseconds (1);
                  else {
                     int x = (mWindmillPulse - WINDMILL_MINPULSE) / 10;
                     //the nice touch here is the square (x*x), to have a feeling of acceleration
                     x = x * x * 10 + 1;
                     delayMicroseconds(x);
                  }
                  
                  //simple version, just go back and forth
                  //delayMicroseconds(mWindmillPulse*2);
                  
                  mWindmillPulse += mWindmillPulseDelta;
                  if (mWindmillPulse >= WINDMILL_MAXPULSE || mWindmillPulse <= 1){
                     mWindmillPulseDelta = -mWindmillPulseDelta;
                  }
               }
               else {
                  //Minutes POV
                  delayMicroseconds(250);            
               }
            }
               
            

            //reverse the pins HIGH/LOW status to turn it off
            digitalWrite(r + WCLK_ROWA, LOW);
            digitalWrite(WCLK_COL1- c, HIGH); //minus because reverse order
         }
      }   
   }

}


#endif //__WordclockLayouts_h__






