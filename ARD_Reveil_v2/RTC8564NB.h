#include "WProgram.h"
#include "wiring.h"

#include "ReveilLCD.h"
#include "ReveilButtons.h"
#include "ymz_melodies.h"

#ifndef __RTC8564_H__
#define __RTC8564_H__

//if defined, trace things to the serial port
//#define DEBUGSERIAL

#include "Wire.h"
//#include "TimerOne.h"

#define RTC8564NB_I2C_ADDRESS B01010001  // This is the I2C address 7bits

extern byte mEepromAlarmHour;
extern byte mEepromAlarmMin;
extern byte mEepromDayMask;
extern byte mEepromAlarmIsOn;

typedef struct {
  byte second;
  byte minute;
  byte hour;
  byte dayOfWeek;
  byte dayOfMonth; 
  byte month;
  byte year;
} 
Date;

Date mLatestReadDate;
byte mSnoozeCountRemaining = 0; //number of times snooze should again occure before dropping the alarm
unsigned long mAlarmRingStart = 0; //when the alarm/snooze started
byte mLatestAlarmFinishedDay = -1; //the last day of week the alarm run and stopped (detect todays stop and no reinit at midnight)
#define ALARM_LENGTH 60000 //60 sec
#define ALARM_SNOOZE_COUNT 5 //snooze 5 times before dropping the alarm 
#define ALARM_SNOOZE_DURATION 5 //snooze 5 minutes
#define ALARM_STOP_DURATION 5000 // 5sec to press 
#define ALARM_TOUCHCOUNT_TO_STOP 2 //gotta touch 2 to stop

volatile boolean mBigButtonPushed = false;

//asks the user if he wants to stop the alarm
void checkStopAlarm(Date& pD){
  byte vTouchCount = 0;

  lcd.cursorTo(1, 0);
  lcd.printIn("  Want to stop  ");  
  lcd.cursorTo(2, 0);
  lcd.printIn("   the alarm ?  ");  

  unsigned long vStartTime = millis();
  while (vTouchCount < ALARM_TOUCHCOUNT_TO_STOP && millis() - vStartTime < ALARM_STOP_DURATION){
    //while not timeout, wait for touch

    //is touched ?
    if (buttonStatus(PIN_BUTTON_BIG)){
      vTouchCount++;

      //message to say to touch again
      lcd.cursorTo(1, 0);
      lcd.printIn(" Touch again to ");  
      lcd.cursorTo(2, 0);
      lcd.printIn("   confirm ...  ");        

      //delay to avoid double touch too quick
      delay(1000);

      //little more time to touch again : reset the counter
      vStartTime = millis();
    }
  }

  if (vTouchCount >= ALARM_TOUCHCOUNT_TO_STOP){
    //stop the alarm
    mSnoozeCountRemaining = 0;
    mLatestAlarmFinishedDay = pD.dayOfWeek;

    //message to say to touch again
    lcd.cursorTo(1, 0);
    lcd.printIn("Have a good day!");  
    lcd.cursorTo(2, 0);
    lcd.printIn("     (^_~)/     ");

    delay(1500);    
  }
}

void blinkLightMany(int pTimes, int pLength){
  for (int i = 0; i < pTimes; i++){
    blinkLight(pLength);
    delay(pLength);
  }
}

//runs the real alarm
void startAlarm(Date& pD){
  byte vTouch = 127;
  //store start time
  mAlarmRingStart = millis();

  //wake up!
  lcd.cursorTo(2, 0);
  lcd.printIn(" ** WAKE UP !** ");

  //get some light
  backlightLighten();

  //bling bling
  blinkLightMany(3, 100);
  lightSet(true);
  
  mBigButtonPushed = false;
  while (
  !mBigButtonPushed
    &&
    millis() < mAlarmRingStart + ALARM_LENGTH 
    ) {
    //loop
    play1MelodyRandom();

    for (int i = 0; i < 10; i++){
      blinkLightMany(1, 100);
      
      if ((vTouch = readTouchKey()) == PIN_BUTTON_BIG){
        mBigButtonPushed = true;
      }
    }
    lightSet(true);
    
    if (!mBigButtonPushed) {
      //not touched

      //blink the time
      for (int i = 0; i < 5; i++){      
        lcd.cursorTo(2 - (i% 2), 0);
        lcd.printIn("                ");
        lcd.cursorTo(1 + (i% 2), 0);
        lcd.printIn(" ** WAKE UP !** ");
        delay(50);
      }
    }
    else {
      //user stopped the snooze ? Want to stop the alarm
      checkStopAlarm(pD);
    }
  }

  //light off
  blinkLightMany(10, 100);
  lightSet(false);

  //store the snooze remaining count
  mSnoozeCountRemaining = max(0, mSnoozeCountRemaining -1);

  //fade to black
  backlightDarken();

  //clear the wake up message
  lcdMakeDefaultScreen();
}

//Checks if the alarm should trigger
boolean checkAlarmRun (Date& pD){
  //could read stuff in EEPROM ... one day

  //now, simple "if mon-fri at 7:45AM"
  //assuming that we will be called at least once within one minute, only work with the snooze count remaining
  if (mLatestAlarmFinishedDay != pD.dayOfWeek
    //&& pD.dayOfWeek >= 1 && pD.dayOfWeek <= 5
  && mEepromAlarmIsOn == 1 
  && bitRead(mEepromDayMask, pD.dayOfWeek-1) == 1 //in the mask filter, that day is 1 (day of week Mon=1)
  && mSnoozeCountRemaining == 0 
    && pD.hour == mEepromAlarmHour && pD.minute == mEepromAlarmMin){
    //RING !!
    mSnoozeCountRemaining = ALARM_SNOOZE_COUNT;
    return true;
  }
  else {
    //should we snooze
    if (mLatestAlarmFinishedDay != pD.dayOfWeek && mSnoozeCountRemaining > 0
      //duration since last snooze is over 1 minute and modulo the snooze duration
    && millis() - mAlarmRingStart > 60000 
      && ((millis() - mAlarmRingStart) / 60000) % ALARM_SNOOZE_DURATION == 0 
      ){
      //RING !!
      return true;
    }   
  }
  return false;
}

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}

//does the offset calculation
Date& offsetDate (Date& pD, int pOffset){
  int vMinutesRaw = 60 * pD.hour + pD.minute + pOffset;

#ifdef DEBUGSERIAL
  Serial.print("pD.minute=");
  Serial.print(pD.minute, DEC);
  Serial.print("  pOffset=");
  Serial.print(pOffset);
  Serial.print("  vMinutesRaw=");
  Serial.println(vMinutesRaw);
#endif 

  pD.hour = (vMinutesRaw / 60) % 24;
  pD.minute = vMinutesRaw % 60;
  
  return pD;
}

//makes a char* to be displayed on the LCD
char* makeDateStringForLCD(char* pBuff, Date& pD){
  char vTempBuff[5];
  //unfortunate compiler optimization forces to manually re-init the char buffer (otherwise buffer overflow)
  //anyway we're lucky, we can use to center the text
  strcpy(pBuff,"    "); 
  strcat(pBuff, dtostrf(pD.hour, 2, 0, vTempBuff));
  strcat(pBuff, ":");
  strcat(pBuff, dtostrf(pD.minute, 2, 0, vTempBuff));
  strcat(pBuff, ":");
  strcat(pBuff, dtostrf(pD.second, 2, 0, vTempBuff));
  strcat(pBuff,"    ");
  return pBuff;
}

// Gets the date and time from the ds1307 and prints result
char* getDate8564(char* pBuff, Date& pD, int pOffset)
{
  // Reset the register pointer

  Wire.beginTransmission(RTC8564NB_I2C_ADDRESS);
  Wire.send(0x02); //move to reg 2
  Wire.endTransmission();

  int vBytesToRead = 7;
  Wire.requestFrom(RTC8564NB_I2C_ADDRESS, vBytesToRead); 

  // A few of these need masks because certain bits are control bits
  // Beware : the sequence is sligthly different from the RTC DS1307 from Maxim
  pD.second     = bcdToDec(Wire.receive() & 0x7f);
  pD.minute     = bcdToDec(Wire.receive() & 0x7f);
  pD.hour       = bcdToDec(Wire.receive() & 0x3f);  // Need to change this if 12 hour am/pm
  pD.dayOfMonth = bcdToDec(Wire.receive() & 0x3f);
  pD.dayOfWeek  = bcdToDec(Wire.receive() & 0x07); //0= sunday
  pD.month      = bcdToDec(Wire.receive() & 0x1f);
  pD.year       = bcdToDec(Wire.receive());

  //save
  mLatestReadDate = pD;
  //offsetting
  pD = offsetDate (pD, pOffset);
  
/*
#ifdef DEBUGSERIAL
  Serial.print(pD.hour, DEC);
  Serial.print(":");
  Serial.print(pD.minute, DEC);
  Serial.print(":");
  Serial.print(pD.second, DEC);
  Serial.print("  ");
  Serial.print(pD.month, DEC);
  Serial.print("/");
  Serial.print(pD.dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(pD.year, DEC);
#endif
*/
  Wire.endTransmission();
  
  makeDateStringForLCD(pBuff, pD);
  
  return pBuff;
}

//checks if stop alarm button was clicked
void callbackCheckButtons(){
  mBigButtonPushed = buttonStatus(PIN_BUTTON_BIG);
}

void setDate8564()                
{
  Wire.beginTransmission(RTC8564NB_I2C_ADDRESS);
  Wire.send(0x02);
  Wire.send(decToBcd(00));    // sec
  Wire.send(decToBcd(50));   //min
  Wire.send(decToBcd(20));   //h   
  Wire.send(decToBcd(23)); //day
  Wire.send(decToBcd(3)); //dow 0= sunday
  Wire.send(decToBcd(12)); //month
  Wire.send(decToBcd(9)); //y
  Wire.endTransmission();
}

void rtcForceStart(){
  Wire.beginTransmission(RTC8564NB_I2C_ADDRESS);
  Wire.send(0x00);
  Wire.send(0x00);
  Wire.endTransmission();
}

void setupRTC8564(){
  //Wire setup to gain access to the RTC
  Wire.begin();
  
  rtcForceStart();
  
  //Timer1.initialize(100000);         // initialize timer1, and set a 1/10th second period
  //Timer1.attachInterrupt(callbackCheckButtons);
}

#endif
