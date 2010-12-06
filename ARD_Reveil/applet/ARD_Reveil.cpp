/*
* ardReveil ~ Arduino powered alarm clock (hardware version 1)
 * Microcode version 0.3
 * 
 * Changes:
 * - v0.3.2: added menu for setting up time offset
 * - v0.3.1: time offset. for unknwon reason, can't set time anymore from pc. So set an offset
 *   saved in ROM.
 *
 */
#include "Speaker.h"
#include "TouchScreen.h"
#include "RTC1307.h"
#include "ReveilLCD.h"

#include "Wire.h"

#include <LCD4Bit.h> 
#include <EEPROM.h>

//set debug mode
//#define REVEIL_DEBUG



#define EEPROM_ADDR_DAYMASK 0
#define EEPROM_ADDR_HOUR 1
#define EEPROM_ADDR_MIN 2
#define EEPROM_ADDR_ALARMONOFF 5
#include "WProgram.h"
void gotoAlarmOnOffMenu();
void gotoSetOffsetMenu ();
void gotoMenuMode();
void readEEPROMData();
void saveEEPROMAlarmOnOff();
void saveEEPROMOffset ();
void saveEEPROMOffset (int pOff);
void endOfHighlight();
void setup();
void loop();
byte mEepromDayMask = 0;
byte mEepromAlarmHour = 0;
byte mEepromAlarmMin = 0;
byte mEepromAlarmIsOn = 1;


boolean mBklIsLightOn = false;
unsigned long mBklStartShowTime = 0;

//this counts the number of time user clicked on the screen while it is in normal mode (not in 
//alarm mode it means)
byte mStackedTouchCount = 0;
//numer of times one have to touch the screen to go to menu mode
#define TOUCHCOUNT_MENU_MODE 4

int mDateOffsetInMinutes; //offset of the date (+/- time) for adjusting RTC
#define EEPROM_DATEOFFSET_MIN 3 //It's a int so uses 2 bytes!!!

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
    while((vTouch = readTouchKey()) == 0){ 
      delay(10); 
    }

    switch (vTouch){
    case TS_KEY_IN:
      //nothing to do...
      break;
    case TS_KEY_PLUS:
    case TS_KEY_MINUS:
      mEepromAlarmIsOn = (mEepromAlarmIsOn == 1? 0 : 1);
      saveEEPROMAlarmOnOff();
      break;
    case TS_KEY_OUT:
    default:
      //OUT!
      return;
    }
  }
}

void gotoSetOffsetMenu (){
  lcd.clear();
  lcd.cursorTo(1, 0);
  lcd.printIn("  adjust time   "); 
  char vTempBuff[5];
  char vBuff[16];

  while(true){
    strcpy(vBuff,"min: "); 
    strcat(vBuff, dtostrf(mDateOffsetInMinutes, 3, 0, vTempBuff));

    lcd.cursorTo(2, 0);
    lcd.printIn(vBuff); 

    //User MUST touch
    byte vTouch = 0;
    //wait for touch
    while((vTouch = readTouchKey()) == 0){ 
      delay(10); 
    }

    switch (vTouch){
    case TS_KEY_IN:
      //nothing to do...
      break;
    case TS_KEY_PLUS:
      mDateOffsetInMinutes++;
      saveEEPROMOffset ();
      break;
    case TS_KEY_MINUS:
      mDateOffsetInMinutes--;
      saveEEPROMOffset ();
      break;
    case TS_KEY_OUT:
    default:
      //OUT!
      return;
    }
  }
}

//enter the menu mode and manage user navigation
void gotoMenuMode(){
  int vCurrentScreen = 0;
  int vSubmenuCount = 3;
  lcd.clear();

  //get some light
  backlightLighten();



  while (vCurrentScreen >= 0){
    lcd.cursorTo(1, 0);
    lcd.printIn("   MENU  MODE   ");
  
    switch(vCurrentScreen){
    case 0: //time adjust
      lcd.cursorTo(2, 0);
      lcd.printIn("  adjust time   "); 
      break;
    case 1: //version
      lcd.cursorTo(2, 0);
      lcd.printIn("  show version  "); 
      break;
    case 2: //version
      lcd.cursorTo(2, 0);
      lcd.printIn("  alarm on/off  "); 
      break;
    default:
      lcd.cursorTo(2, 0);
      lcd.printIn(" ???unknown???  "); 
    }

    //User MUST touch
    byte vTouch = 0;
    //wait for touch
    while((vTouch = readTouchKey()) == 0){ 
      delay(10); 
    }
    
#ifdef REVEIL_DEBUG
    Serial.print("pressed:");
    Serial.println(vTouch);
#endif   

    switch (vTouch){
    case TS_KEY_IN:
      switch(vCurrentScreen){
      case 0: //time adjust
        gotoSetOffsetMenu();
        break;
      case 1: //version
        showVersion();
        delay(2000);
        break;
      case 2: //alarm onoff
        gotoAlarmOnOffMenu();
        break;
      default:
        lcd.cursorTo(2, 0);
        lcd.printIn(" ???unknown???  "); 
      }    
      break;
    case TS_KEY_PLUS:
      vCurrentScreen = (vCurrentScreen+1) % vSubmenuCount;
      break;
    case TS_KEY_MINUS:
      vCurrentScreen = (vCurrentScreen == 0 ? 0 : vCurrentScreen-1) % vSubmenuCount;
      break;
    case TS_KEY_OUT:
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
}

void saveEEPROMAlarmOnOff(){
  EEPROM.write(EEPROM_ADDR_ALARMONOFF, mEepromAlarmIsOn);
}
void saveEEPROMOffset (){ 
  saveEEPROMOffset(mDateOffsetInMinutes); 
}
void saveEEPROMOffset (int pOff){
  EEPROM.write(EEPROM_DATEOFFSET_MIN, (byte)(pOff >> 8));
  EEPROM.write(EEPROM_DATEOFFSET_MIN+1, (byte)(pOff & 0x00FF));
}


//cleanup to do when screen goes back to black
void endOfHighlight(){
  mBklIsLightOn = false;

  //fade to black
  backlightDarken();

  //reset the touch count    
  mStackedTouchCount = 1; 
}


void setup() { 
  /*
  // That arduino pro board is 16 mHz BUT the bootloader is made for 8MHz so force the 
   // Arduino to run half speed (force 8MHz)
   CLKPR = (1<<CLKPCE);
   CLKPR = 1; // Divide by 2 
   */

  Wire.begin();

  //setDateDs1307();

  lcd.init();
  lcd.clear();

  //reset to default
  analogWrite(PIN_BKL, BKL_DEFAULT_VAL);

  //Set the sound out pin to output mode
  pinMode(SOUNDOUT_PIN,OUTPUT);

  readEEPROMData();

  //start the show!  
  DoLightsBlingBling();
  splashScreen();

  PlayMusicString(tididit,SOUNDOUT_PIN);

  //prepare display
  lcdMakeDefaultScreen();

#ifdef REVEIL_DEBUG
  //PlayMusicString(playlist[random(0, 4)],SOUNDOUT_PIN);
  //startAlarm();
  Serial.begin(9600);


  //reset eeprom settings
  //EEPROM.write(EEPROM_ADDR_DAYMASK, 31); //mon-fri
  //EEPROM.write(EEPROM_ADDR_HOUR, 07);
  //EEPROM.write(EEPROM_ADDR_MIN, 45);
  //readEEPROMData();

  //reset time offset
  //saveEEPROMOffset(14);
  //readEEPROMData();

  Serial.println("== EEPROM DATA ==");
  Serial.print(mEepromDayMask,DEC);
  Serial.print(" ");
  Serial.print(mEepromAlarmHour,DEC);
  Serial.print(":");
  Serial.print(mEepromAlarmMin,DEC);
  Serial.println("");

  Serial.print("the offset:");
  Serial.println(mDateOffsetInMinutes,DEC);
#endif
}



void loop() { 
  //read count once per sec at max
  if (mReadCount >= TIME_READ_TRIGGER){
    char vBuff[20];
    Date vD;
    //lcd.clear();
    lcd.cursorTo(1, 0);
    lcd.printIn(getDateDs1307(vBuff, vD, mDateOffsetInMinutes));

    mReadCount = 0;

    if (checkAlarmRun (vD)){
      //run the alarm
      startAlarm(vD);
    }

  }

  //turn off light?
  if (mBklIsLightOn && millis() - mBklStartShowTime > BKL_SHOW_TIME){
    endOfHighlight();
  }

  //is touched ?
  byte vTouch = 0;
  if ((vTouch = readTouchKey()) != 0){
    //touched!
#ifdef REVEIL_DEBUG
    Serial.print("Touched KEY: ");
    Serial.println(vTouch);
#endif    
    if (!mBklIsLightOn){
      //get some light
      backlightLighten();
    }
    //save the info light is on and the show time
    mBklIsLightOn = true;
    mBklStartShowTime = millis();
    mStackedTouchCount += 1; //start counting nmber of times touched

    if (mStackedTouchCount >= TOUCHCOUNT_MENU_MODE){
      //go to menu mode
      gotoMenuMode();
      //and when out, back to normal mode
      endOfHighlight();
    }
  }


  //next round...
  mReadCount += 1;
  delay(TOUCH_READ_PERIOD);
}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

