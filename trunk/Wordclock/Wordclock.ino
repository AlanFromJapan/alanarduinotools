#include "DS3234.h"
#include "WordclockShared.h"
#include "DS3231.h"
#include "WordclockLeds.h"
#include "WordclockLayouts.h"

//DS3234.h requires this to be in the main .ino file.
#include "SPI.h"
//DS3231.h requires this to be in the main .ino file.
#include "Wire.h"

#include "EEPROM.h"

#define BUTTON_ANALOG_PIN 2
#define MAP_MATRIX_MFUNC(p) MapTimeInLedMatrix_LaFrance(p)
#define DRAW_MATRIX_FUNC() drawLedMatrix2x1()
#define SETUP_MATRIX() setupLedMatrix2x1()

void setup() {
   SETUP_MATRIX();
   
   setupDS3231();

   //do this init just once, to make sure there is something "coherent" in the RTC
   if (EEPROM.read(1) != 1) {
      setControlRegisters();
      setDateDS3231();

      EEPROM.write(1, 1); 
   }


}

//second,minute,hour,null,day,month,year
void loop() { 
   //if button is pushed, go to some subroutine and change time
   checkButtonTimeSet();

   //second,minute,hour,null,day,month,year
   int vTimeArray[7];

   Date3231 vD3231;
   getDateDS3231(vD3231);
   
   //ReadTime(vDate);
   vTimeArray[0] = vD3231.second;
   vTimeArray[1] = vD3231.minute;
   vTimeArray[2] = vD3231.hour;
   
   //Uncomment the following line for a demo mode with fast time
   //ReadTimeArray_Fake(&vTimeArray[0], 10);

   //Draw the in-memory matrix (change constant at the top of the file)
   MAP_MATRIX_MFUNC(vTimeArray);

   //Draw the matrix in memory on the leds
   DRAW_MATRIX_FUNC();
   
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


//if button is pressed, vamp the execution loop and other set time routine according button pressed
void checkButtonTimeSet(){
   //second,minute,hour,null,day,month,year
   int vTimeArray[7];

   if (readButtonPressed(1024, 20)){
      //debouncing on the cheap
      delay (300);

      //stage=0 hour,stage 1 = minutes
      int vStage = 0; 
      while (vStage < 2){
         //pressed the + button?
         if (readButtonPressed(512, 20)){
            //debouncing on the cheap
            delay (300);

            Date3231 vD3231;
            getDateDS3231(vD3231);

            switch(vStage){
            case 0 : //hours
               vD3231.hour = (vD3231.hour + 1) % 24;
               break;
            case 1 : //minutes
               vD3231.minute = (vD3231.minute + 1) % 60;
               break;                
            }

            //and save the time
            setDateDS3231(vD3231);
         }

         //pressed the set button
         if (readButtonPressed(1024, 20)){      
            //debouncing on the cheap
            delay (300);

            vStage++;
         }

         //at last, display the time
         Date3231 vD3231;
         getDateDS3231(vD3231);
         vTimeArray[0] = vD3231.second;
         vTimeArray[1] = vD3231.minute;
         vTimeArray[2] = vD3231.hour;

         //Draw the in-memory matrix (change constant at the top of the file)
         MAP_MATRIX_MFUNC(vTimeArray);
      
         //Draw the matrix in memory on the leds
         DRAW_MATRIX_FUNC();
   
         //a little rest longer than usual that will "blink"
         delay(50);
      }
   }
}

boolean readButtonPressed (int pMidValue, int pTolerance){
   int vVal = analogRead(BUTTON_ANALOG_PIN);

   return vVal <= pMidValue + pTolerance 
      && vVal >= pMidValue - pTolerance;
}






