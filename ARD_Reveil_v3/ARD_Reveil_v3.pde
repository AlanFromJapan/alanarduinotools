/*
* ARD_Reveil_v3
 * -------------
 * http://kalshagar.wikispaces.com/ardReveil+v3
 * New version of the ardReveil : without arduino (just the chip), but with a proximity infrared sensor
 * RTC module this time is the RDC DS3232 from Maxim (extremely good stuff, that Maxim kindly sent me in 2 ex : THANK YOU GUYS !).
 *
 *
 * VERSION HISTORY
 * 3.0 (13/03/2010) : initial version
 *
 */
 
//if defined, trace things to the serial port
//#define DEBUGSERIAL

#include "RTCDS3232.h"
#include "ReveilLCD.h"

#include "Wire.h"
#include "LCD4Bit.h"
#include "EEPROM.h"

#define PIN_IRSENSOR 0 //ANALOG

void setup() {

  CLKPR = (1<<CLKPCE);
  CLKPR = 0; // No divide!   
  
  
  /*
  mEepromDayMask = B00011111;
   mEepromAlarmHour = 19;
   mEepromAlarmMin = 20; 
   mEepromAlarmIsOn = 1;
   saveEEPROMAlarmTime();
   saveEEPROMAlarmOnOff();
   */

  // in that order
  // 1: read the EEPROM data
  //readEEPROMData();
  // 2: setup the RTC
  setupRTC3232();
  // 3: setup the shift register
  //setupCD4015();
  // 4: setup the Yamaha YMZ294
  //setupYMZ294();
  // 5: setup the LCD
  //setDefaultBrightness(mEepromLcdBrightness);
  setupLCD();
  // 6: setup buttons
  //setupButtons();
  // ... and roll
  
  
#ifdef DEBUGSERIAL
  Serial.begin(9600);
#endif  
}

char vBuff[17];
int vNewVal = 0;
Date vD;
void loop() {
  //Write time
  getDateDS3232(vBuff, vD, 0);  

  
  int vPreviousVal = vNewVal;
  vNewVal = analogRead(PIN_IRSENSOR);
  if (vNewVal > 100){
    vNewVal = map(vNewVal, 100, 700, 0, 255);
    analogWrite(LCD_BACKLIGHT, vNewVal);
  }
  else {
    if (vPreviousVal > 0){
      //it was bright, now it becomes dark
      analogWrite(LCD_BACKLIGHT, 0);
      lcdMakeDefaultScreen();
    }
    vNewVal = 0;
  }
  
  lcdWriteLine(1, vBuff);

  delay(50);
}
