#include "WProgram.h"
#include "wiring.h"

#ifndef __REVEILLCD_H__
#define __REVEILLCD_H__

#include <LCD4Bit.h> 

#define PIN_BKL 9
#define BKL_DEFAULT_VAL 0

#define PIN_LED_WHITE 10
#define PIN_LED_GREEN 11

extern byte mEepromAlarmHour;
extern byte mEepromAlarmMin;
extern byte mEepromDayMask;

//create object to control an LCD.  
//number of lines in display=1
LCD4Bit lcd = LCD4Bit(2); 


//manage the 2 leds white and green
void DoLightsBlingBling(){
  for (int i = 0; i <= 16; i++){
    analogWrite(PIN_LED_WHITE, (i % 2) * 255);
    analogWrite(PIN_LED_GREEN, (i % 2) * 255);
    delay(50);
  }

  //turn off
  analogWrite(PIN_LED_WHITE, 0);
  analogWrite(PIN_LED_GREEN, 0);
}



//Background light : ligten 
void backlightLighten(){
  //lighten ...
  for (int i = BKL_DEFAULT_VAL; i < 256 ; i += 15){
    analogWrite(PIN_BKL, i);
    delay(25);
  }
}

//Background light : darken
void backlightDarken(){
  //darken ...
  for (int i = 255; BKL_DEFAULT_VAL < i ; i -= 15){
    analogWrite(PIN_BKL, i);
    delay(25);
  }

  //reset to default
  analogWrite(PIN_BKL, BKL_DEFAULT_VAL);
}

void showVersion(){
  lcd.cursorTo(1, 0);
  lcd.printIn("    ardReveil   ");
  lcd.cursorTo(2, 0);
  lcd.printIn("    v 0.3.2     "); 
}


//prepares the display for the usual screen with time.
//not meant to be called each second
void lcdMakeDefaultScreen(){
  //lcd.clear();
  lcd.cursorTo(1, 0);
  lcd.printIn("                ");
  lcd.cursorTo(2, 0);
  lcd.printIn("     (^_~)b     ");
}

//does some sort of splash screen
void splashScreen(){
  lcd.clear();
  delay(1000);

  //get some light
  backlightLighten();

  //show the version
  showVersion();

  delay(1000);
  lcd.leftScroll(16, 50);

  //unfortunate compiler optimization forces to manually re-init the char buffer (otherwise buffer overflow)
  //anyway we're lucky, we can use to center the text
  char vTempBuff[5];
  char vBuff[16];
  strcpy(vBuff,"      "); 
  strcat(vBuff, dtostrf(mEepromAlarmHour, 2, 0, vTempBuff));
  strcat(vBuff, ":");
  strcat(vBuff, dtostrf(mEepromAlarmMin, 2, 0, vTempBuff));
  
  lcd.printIn("   Wake-up at   ");
  lcd.cursorTo(2, 0);
  lcd.printIn(vBuff); 

  delay(1000);
  lcd.leftScroll(16, 50);

  lcdMakeDefaultScreen();
  
  //fade to black
  backlightDarken();
}


#endif //__REVEILLCD_H__
