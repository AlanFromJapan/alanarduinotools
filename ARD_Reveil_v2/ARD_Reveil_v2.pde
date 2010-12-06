/*
* ARD_Reveil_v2
 * -------------
 * http://kalshagar.wikispaces.com/ardReveil+v2
 * New version of the ardReveil that uses a YMZ294 for generating music.
 * RTC module this time is the RDC 8564 from Epson (nice board from Atsukidenshi).
 *
 *
 * VERSION HISTORY
 * 2.0 (23/12/2009) : initial version
 *
 */
//if defined, trace things to the serial port
//#define DEBUGSERIAL

#include "RTC8564NB.h"
#include "ReveilLCD.h"
#include "YMZ294.h"
#include "ymz_melodies.h"
#include "ReveilButtons.h"

#include "Wire.h"
#include "LCD4Bit.h"
#include "EEPROM.h"
//#include "TimerOne.h"

//-----------------------------------------------------------------------------------------------
//--                         EEPROM SETTINGS
//-----------------------------------------------------------------------------------------------
#define EEPROM_ADDR_DAYMASK 0
#define EEPROM_ADDR_HOUR 1
#define EEPROM_ADDR_MIN 2
#define EEPROM_ADDR_ALARMONOFF 5
#define EEPROM_ADDR_MUSICONCLICK 6
#define EEPROM_ADDR_LCDLIGHT 7 
byte mEepromDayMask = 0;
byte mEepromAlarmHour = 0;
byte mEepromAlarmMin = 0;
byte mEepromAlarmIsOn = 1;
byte mEepromMusicOnClick = 1;
int mEepromLcdBrightness = 0;

int mDateOffsetInMinutes; //offset of the date (+/- time) for adjusting RTC
#define EEPROM_DATEOFFSET_MIN 3 //It's a int so uses 2 bytes!!!
//-----------------------------------------------------------------------------------------------


//reads config saved in the EEPROM
void readEEPROMData(){
  mEepromDayMask = EEPROM.read(EEPROM_ADDR_DAYMASK);
  mEepromAlarmHour = EEPROM.read(EEPROM_ADDR_HOUR);
  mEepromAlarmMin = EEPROM.read(EEPROM_ADDR_MIN);

  //offset is a int on 2 bytes!
  mDateOffsetInMinutes = EEPROM.read(EEPROM_DATEOFFSET_MIN);
  mDateOffsetInMinutes = mDateOffsetInMinutes << 8;
  mDateOffsetInMinutes |= EEPROM.read(EEPROM_DATEOFFSET_MIN+1);

  mEepromAlarmIsOn = EEPROM.read(EEPROM_ADDR_ALARMONOFF);
  mEepromMusicOnClick = EEPROM.read(EEPROM_ADDR_MUSICONCLICK);
  
  mEepromLcdBrightness = EEPROM.read(EEPROM_ADDR_LCDLIGHT);
}
void saveEEPROMAlarmOnOff(){
  EEPROM.write(EEPROM_ADDR_ALARMONOFF, mEepromAlarmIsOn);
}
void saveEEPROMLcdBrightness(){
  EEPROM.write(EEPROM_ADDR_LCDLIGHT, mEepromLcdBrightness);
}
void saveEEPROMMusicOnClick(){
  EEPROM.write(EEPROM_ADDR_MUSICONCLICK, mEepromMusicOnClick);
}
void saveEEPROMOffset (){ 
  saveEEPROMOffset(mDateOffsetInMinutes); 
}
void saveEEPROMOffset (int pOff){
  EEPROM.write(EEPROM_DATEOFFSET_MIN, (byte)(pOff >> 8));
  EEPROM.write(EEPROM_DATEOFFSET_MIN+1, (byte)(pOff & 0x00FF));
}
void saveEEPROMAlarmTime (){ 
  EEPROM.write(EEPROM_ADDR_DAYMASK, mEepromDayMask);
  EEPROM.write(EEPROM_ADDR_HOUR, mEepromAlarmHour);
  EEPROM.write(EEPROM_ADDR_MIN, mEepromAlarmMin);  
}



//plays ALL known music
void jukeBox(){
  lcd.clear();
  lcdWriteLine(1, "    jukebox!    ");
  setVolumeYMZ294(0xff);  
  for (int i = 0; i < MELODIES_COUNT; i++){
    lcdWriteLine(2, melodies[i].mMeloName);
    melodies[i].mPlay();
  }
  setVolumeYMZ294(0x00); 
  lcdMakeDefaultScreen();
}



void gotoBacklightMenu(){
  lcd.clear();
  lcd.cursorTo(1, 0);
  lcd.printIn("**  backlight **"); 
  lcd.cursorTo(2, 0);
  lcd.printIn("BIG=+      RED=-");
  int vTemp = mEepromLcdBrightness;
  
  //force brightness to normal screen one
  analogWrite(LCD_BACKLIGHT, mEepromLcdBrightness);
      
  while(true){
    //User MUST touch
    byte vTouch = 0;
    //wait for touch
    while((vTouch = readTouchKey()) == 127){ 
      delay(10); 
    }

    switch (vTouch){
    case PIN_BUTTON_RED:    // -
    case PIN_BUTTON_BIG:    // +
      vTemp = vTemp + (vTouch == PIN_BUTTON_RED ? -5 : 5);
      if (vTemp < 0) vTemp = 0;
      if (vTemp > 255) vTemp = 255;
      mEepromLcdBrightness = vTemp;
      setDefaultBrightness(mEepromLcdBrightness);
      analogWrite(LCD_BACKLIGHT, mEepromLcdBrightness);
      //delay otherwise bounce phnomenon
      delay(200);
      break;
    case PIN_BUTTON_BLACK:
      //save
      saveEEPROMLcdBrightness();
      //return to full light
      backlightLighten();
      return;
    default:
      //OUT!
      return;
    }
  }
}

void gotoMusicOnClickOnOffMenu(){
  lcd.clear();
  lcd.cursorTo(1, 0);
  lcd.printIn("  music on/off  "); 

  while(true){
    lcd.cursorTo(2, 0);
    if (mEepromMusicOnClick == 1){
      lcd.printIn("  music is ON   "); 
    }
    else {
      lcd.printIn("  music is OFF  "); 
    }

    //User MUST touch
    byte vTouch = 0;
    //wait for touch
    while((vTouch = readTouchKey()) == 127){ 
      delay(10); 
    }

    switch (vTouch){
    case PIN_BUTTON_BIG:
      //nothing to do...
      break;
    case PIN_BUTTON_BLACK:
      mEepromMusicOnClick = (mEepromMusicOnClick == 1? 0 : 1);
      saveEEPROMMusicOnClick();
      break;
    case PIN_BUTTON_RED:
    default:
      //OUT!
      return;
    }
  }
}


void gotoAlarmOnOffMenu(){
  lcd.clear();
  lcd.cursorTo(1, 0);
  lcd.printIn("  alarm on/off  "); 

  while(true){
    lcd.cursorTo(2, 0);
    if (mEepromAlarmIsOn == 1){
      lcd.printIn("  alarm is ON   "); 
    }
    else {
      lcd.printIn("  alarm is OFF  "); 
    }

    //User MUST touch
    byte vTouch = 0;
    //wait for touch
    while((vTouch = readTouchKey()) == 127){ 
      delay(10); 
    }

    switch (vTouch){
    case PIN_BUTTON_BIG:
      //nothing to do...
      break;
    case PIN_BUTTON_BLACK:
      mEepromAlarmIsOn = (mEepromAlarmIsOn == 1? 0 : 1);
      saveEEPROMAlarmOnOff();
      break;
    case PIN_BUTTON_RED:
    default:
      //OUT!
      return;
    }
  }
}

//set time or alarm time
void gotoSetOffsetMenu (boolean pIsTime){
  lcd.clear();
  lcd.cursorTo(1, 0);
  if (pIsTime){
    lcd.printIn("*  adjust time *"); 
  }
  else {
    lcd.printIn("* adjust alarm *"); 
  }
  char vTempBuff[5];
  char vBuff[16];

  Date vAlarmTime;
  vAlarmTime.hour = 0;
  vAlarmTime.minute = 0;
  vAlarmTime.second = 0;
  
  int vAlarmTimeInMin = 0;
  vAlarmTimeInMin = mEepromAlarmHour * 60 + mEepromAlarmMin;

  Date vDisplayDate;
  int vTempOffset = (pIsTime ? mDateOffsetInMinutes : vAlarmTimeInMin);
  byte vCurrentMember = 0; //0=h, 1=m

  while(true){
    //prepare time to display
    vDisplayDate = (pIsTime ? mLatestReadDate : vAlarmTime);
    vDisplayDate = offsetDate(vDisplayDate, vTempOffset);
    //           "    hh:mm:ss    "); 
    makeDateStringForLCD(vBuff, vDisplayDate);
    //write on lcd
    lcd.cursorTo(2, 0);
    lcd.printIn(vBuff); 
    //move cursor, make blink, cursor visible
    lcd.commandWrite(0x0F);
    delayMicroseconds(60);
    lcd.cursorTo(2, 4 + vCurrentMember * 3);

    //User MUST touch
    byte vTouch = 127;
    //wait for touch
    while((vTouch = readTouchKey()) == 127){ 
      delay(10); 
    }

    switch (vTouch){
    case PIN_BUTTON_BIG: //+
    case PIN_BUTTON_RED: //-
      if (vCurrentMember == 0){
        vTempOffset = vTempOffset + (vTouch == PIN_BUTTON_BIG ? (int)60 : (int)-60);
      }
      else {
        vTempOffset = vTempOffset + (vTouch == PIN_BUTTON_BIG ? (int)1 : (int)-1);
      }

      vDisplayDate = (pIsTime ? mLatestReadDate : vAlarmTime);
      vDisplayDate = offsetDate(vDisplayDate, vTempOffset);
      if (vDisplayDate.hour == 24){
        vTempOffset = (vTempOffset % 60) - vDisplayDate.hour;
      }
      break;
    case PIN_BUTTON_BLACK: //next or out
      vCurrentMember += 1;

      if (vCurrentMember >= 2){
        //save and return
        if (pIsTime){
          //save time
          mDateOffsetInMinutes = vTempOffset;
          saveEEPROMOffset ();
        }
        else {
          //save alarm
          mEepromAlarmHour = vDisplayDate.hour;
          mEepromAlarmMin = vDisplayDate.minute;
          saveEEPROMAlarmTime();
        }
        readEEPROMData();
        return;
      }
      break;
    }
  }
}


//enter the menu mode and manage user navigation
void gotoMenuMode(){
  int vCurrentScreen = 0;
  int vSubmenuCount = 7;
  lcd.clear();

  while (vCurrentScreen >= 0){
    lcd.cursorTo(1, 0);
    lcd.printIn("== MENU  MODE ==");

    switch(vCurrentScreen){
    case 0: //time adjust
      lcd.cursorTo(2, 0);
      lcd.printIn("  adjust time   "); 
      break;
    case 1: //version
      lcd.cursorTo(2, 0);
      lcd.printIn("  show version  "); 
      break;
    case 2: //alarm on/off
      lcd.cursorTo(2, 0);
      lcd.printIn("  alarm on/off  "); 
      break;
    case 3: //time adjust
      lcd.cursorTo(2, 0);
      lcd.printIn("  adjust alarm  "); 
      break;
    case 4: //music on click on/off
      lcd.cursorTo(2, 0);
      lcd.printIn("  music on/off  "); 
      break;
    case 5: //jukebox
      lcd.cursorTo(2, 0);
      lcd.printIn("     jukebox    "); 
      break;
    case 6: //light
      lcd.cursorTo(2, 0);
      lcd.printIn("    backlight   "); 
      break;
    default:
      lcd.cursorTo(2, 0);
      lcd.printIn(" ???unknown???  "); 
    }

    //User MUST touch
    byte vTouch = 127;
    //wait for touch
    while((vTouch = readTouchKey()) == 127){ 
      delay(10); 
    }

#ifdef DEBUGSERIAL
    Serial.print("pressed:");
    Serial.println(vTouch);
#endif   

    switch (vTouch){
    case PIN_BUTTON_BIG:
      switch(vCurrentScreen){
      case 0: //time adjust
        gotoSetOffsetMenu(true);
        break;
      case 1: //version
        showVersion();
        delay(2000);
        break;
      case 2: //alarm onoff
        gotoAlarmOnOffMenu();
        break;
      case 3: //alarm adjust
        gotoSetOffsetMenu(false);
        break;
      case 4: //music onoff
        gotoMusicOnClickOnOffMenu();
        break;
      case 5: //jukebox
        jukeBox();
        break;
      case 6: //light
        gotoBacklightMenu();
        break;
      default:
        lcd.cursorTo(2, 0);
        lcd.printIn(" ???unknown???  "); 
      }    
      break;
    case PIN_BUTTON_BLACK:
      vCurrentScreen = (vCurrentScreen+1) % vSubmenuCount;
      break;
    case PIN_BUTTON_RED: //TS_KEY_OUT:
    default:
      if (vCurrentScreen == 0){
        vCurrentScreen =-1; //out
      }
      else {
        vCurrentScreen = 0; //back to root
      }
    }
  }

  //cleanup
  lcdMakeDefaultScreen();
}

void setup() {
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
  readEEPROMData();
  // 2: setup the RTC
  setupRTC8564();
  // 3: setup the shift register
  setupCD4015();
  // 4: setup the Yamaha YMZ294
  setupYMZ294();
  // 5: setup the LCD
  setDefaultBrightness(mEepromLcdBrightness);
  setupLCD();
  // 6: setup buttons
  setupButtons();
  // ... and roll

  blinkLightMany(10, 100);
  lightSet(false);
  
#ifdef DEBUGSERIAL
  Serial.begin(9600);
#endif  
}

char vBuff[17];
Date vD;
void loop() {
  //Write time
  lcdWriteLine(1, getDate8564(vBuff, vD, mDateOffsetInMinutes));
  if (checkAlarmRun (vD)){
    //run the alarm
    startAlarm(vD);
  }

  //pressed the big button on the top?
  if (buttonStatus(PIN_BUTTON_BIG)){
    backlightLighten();
    if (mEepromMusicOnClick != 0) {
      play1MelodyRandom();
    }
    else {
      delay(1000);
    }
    backlightDarken();
    //reset screen
    lcdMakeDefaultScreen();
  }

  //pressed the red button at the back?
  if (buttonStatus(PIN_BUTTON_RED)){
    backlightLighten();
    gotoMenuMode();
    backlightDarken();
  }

  //reset screen every 7 minutes or so, for a little fun
  if (mLatestReadDate.second == 7 && mLatestReadDate.minute % 7 == 0){
    lcdMakeDefaultScreen();
    delay(1000);
  }
  
  delay(100);
}
//*****************************************************The End***********************
