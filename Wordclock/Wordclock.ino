/********************************************************************************
**
** Wordclock by AlanFromJapan
** http://kalshagar.wikispaces.com/WordClock
**
********************************************************************************/



/********************************************************************************
**
** Choose which display to use
** (must be before the includes because the #define is used in the .h)
**
********************************************************************************/
#define USE_DISPLAY_NUMITRON
//#define USE_DISPLAY_KOREA3
//#define USE_DISPLAY_LAFRANCE1
//#define USE_DISPLAY_JAPANESE1
//#define USE_DISPLAY_THEORIGINAL
//#define USE_DISPLAY_BCD1
//#define USE_DISPLAY_WINDMILL

//Uncomment one of the two to indicate which RTC you use
#define RTC_DS3231
//#define RTC_DS3234


//List of includes
#include "DS3234.h"
#include "DS3231.h"
#include "WordclockShared.h"
#include "WordclockLeds.h"
#include "WordclockLayouts.h"
#include "WordclockTinyNumitron.h"
#include "EEPROM.h"

//DS3234.h requires this to be in the main .ino file.
#include "SPI.h"
//DS3231.h requires this to be in the main .ino file.
#include "Wire.h"




void setup() {
   SETUP_MATRIX();

#ifdef RTC_DS3231
   setupDS3231(false);
#endif //RTC_DS3231

#ifdef RTC_DS3234
   setupDS3234(true);
#endif //RTC_DS3234

   
   //do this init just once, to make sure there is something "coherent" in the RTC
    if (EEPROM.read(1) != 1) {

#ifdef RTC_DS3231      
    setControlRegisters();
    setDateDS3231();
#endif 

#ifdef RTC_DS3234
   SetTimeDate(25,05,2013,18,14,00);
#endif //RTC_DS3234

    EEPROM.write(1, 1); 
    }


}


//second,minute,hour,null,day,month,year
void readTimeArray(Date& pTime){
  
#ifdef RTC_DS3231
   getDateDS3231(pTime);
#endif //RTC_DS3231

#ifdef RTC_DS3234
    ReadTime(pTime);
#endif //RTC_DS3234

}

//second,minute,hour,null,day,month,year
void setTimeArray (Date& pTime){
    
#ifdef RTC_DS3231

   setDateDS3231(pTime);
#endif //RTC_DS3231
#ifdef RTC_DS3234

   SetTimeDate(pTime.dayOfMonth, pTime.month, pTime.year, pTime.hour, pTime.minute, pTime.second);
#endif //RTC_DS3234

}


//second,minute,hour,null,day,month,year
void loop() { 
   //if button is pushed, go to some subroutine and change time
   checkButtonTimeSet();

   //second,minute,hour,null,day,month,year
   Date vD;
   readTimeArray(vD);

   //Uncomment the following line for a demo mode with fast time
   //readTimeArray_Fake(vD, 100);

   //Draw the in-memory matrix (change constant at the top of the file)
   MAP_MATRIX_MFUNC(vD);

   //Draw the matrix in memory on the leds
   DRAW_MATRIX_FUNC();

   //a little rest
   delay(5);
}


//Reads FAKE time (time passes fast, for test purpose)
//second,minute,hour,null,day,month,year	
void readTimeArray_Fake(Date& pD, int SpeedFactor){
   //(1/SpeedFactor) sec is one minute
   unsigned long vTime = ((millis()* (unsigned long)SpeedFactor) / ((unsigned long)1000)) % (unsigned long)(24 * 60);

   pD.second = 0;
   pD.minute = (int)(vTime % 60);
   pD.hour = (int)(vTime / 60);
   //ignore the rest...
}


//if button is pressed, vamp the execution loop and other set time routine according button pressed
void checkButtonTimeSet(){ 
   Date vD;

   if (readButtonPressed(1024, 20)){
      //debouncing on the cheap
      delay (300);

      //stage=0 hour,stage 1 = minutes
      int vStage = 0; 
      while (vStage < 2){
         //pressed the + button?
         if (readButtonPressed(512, 20)){
            //debouncing on the cheap
            delay (150);

            //read current time
            readTimeArray(vD);

            switch(vStage){
            case 0 : //hours
               vD.hour = (vD.hour + 1) % 24;
               break;
            case 1 : //minutes
               vD.minute = (vD.minute + 1) % 60;
               break;                
            }

            //and save the time
            setTimeArray(vD);
         }

         //pressed the set button
         if (readButtonPressed(1024, 20)){      
            //debouncing on the cheap
            delay (300);

            vStage++;
         }

        //read current time
        readTimeArray(vD);
            
         //at last, display the time
         //Draw the in-memory matrix (change constant at the top of the file)
         MAP_MATRIX_MFUNC(vD);
      
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







