#include "WProgram.h"
#include "wiring.h"

#include "ReveilLCD.h"

#ifndef __RTC1307_H__
#define __RTC1307_H__

#include "Wire.h"

//set debug mode
//#define REVEIL_DEBUG

#define BKL_SHOW_TIME 3000 // how long stay on on touch

#define DS1307_I2C_ADDRESS 0x68  // This is the I2C address

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
byte mSnoozeCountRemaining = 0; //number of times snooze should again occure before dropping the alarm
unsigned long mAlarmRingStart = 0; //when the alarm/snooze started
byte mLatestAlarmFinishedDay = -1; //the last day of week the alarm run and stopped (detect todays stop and no reinit at midnight)
#define ALARM_LENGTH 60000 //60 sec
#define ALARM_SNOOZE_COUNT 5 //snooze 5 times before dropping the alarm 
#define ALARM_SNOOZE_DURATION 5 //snooze 5 minutes
#define ALARM_STOP_DURATION 5000 // 5sec to press 
#define ALARM_TOUCHCOUNT_TO_STOP 2 //gotta touch 2 to stop

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

// Gets the date and time from the ds1307 and prints result
char* getDateDs1307(char* pBuff, Date& pD, int pOffset)
{
  // Reset the register pointer
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.send(0x00);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);

  // A few of these need masks because certain bits are control bits
  pD.second     = bcdToDec(Wire.receive() & 0x7f);
  pD.minute     = bcdToDec(Wire.receive());
  pD.hour       = bcdToDec(Wire.receive() & 0x3f);  // Need to change this if 12 hour am/pm
  pD.dayOfWeek  = bcdToDec(Wire.receive());
  pD.dayOfMonth = bcdToDec(Wire.receive());
  pD.month      = bcdToDec(Wire.receive());
  pD.year       = bcdToDec(Wire.receive());

#ifdef REVEIL_DEBUG
  Serial.print("ORIGINAL: ");
  Serial.print(pD.hour,DEC);   
  Serial.print(":");   
  Serial.print(pD.minute,DEC);   
#endif     

  //offsetting
  int vMinutesRaw = pD.minute + pOffset;
  pD.hour = (pD.hour + vMinutesRaw / 60) % 24; //maybe error here with NEGATIVE offset
  pD.minute = vMinutesRaw % 60;

#ifdef REVEIL_DEBUG
  Serial.print("  /  AFTER OFFSET:");   
  Serial.print(pD.hour,DEC);   
  Serial.print(":");   
  Serial.print(pD.minute,DEC);            
  Serial.println(""); 
#endif     

  char vTempBuff[5];
  //unfortunate compiler optimization forces to manually re-init the char buffer (otherwise buffer overflow)
  //anyway we're lucky, we can use to center the text
  strcpy(pBuff,"    "); 
  strcat(pBuff, dtostrf(pD.hour, 2, 0, vTempBuff));
  strcat(pBuff, ":");
  strcat(pBuff, dtostrf(pD.minute, 2, 0, vTempBuff));
  strcat(pBuff, ":");
  strcat(pBuff, dtostrf(pD.second, 2, 0, vTempBuff));

  //Wire.endTransmission();

  return pBuff;
}


//asks the user if he wants to stop the alarm
void checkStopAlarm(Date& pD){
  //light to be sure it's readable
  analogWrite(PIN_BKL, 255);
  byte vTouchCount = 0;

  lcd.cursorTo(1, 0);
  lcd.printIn("  Want to stop  ");  
  lcd.cursorTo(2, 0);
  lcd.printIn("   the alarm ?  ");  

  unsigned long vStartTime = millis();
  while (vTouchCount < ALARM_TOUCHCOUNT_TO_STOP && millis() - vStartTime < ALARM_STOP_DURATION){
    //while not timeout, wait for touch

    //is touched ?
    int x;
    int y;
    if (readTouch(x, y)){
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
    PlayMusicString(tididit,SOUNDOUT_PIN);

    delay(1500);    
  }
}

//runs the real alarm
void startAlarm(Date& pD){
  //store start time
  mAlarmRingStart = millis();

  //wake up!
  lcd.cursorTo(2, 0);
  lcd.printIn(" ** WAKE UP !** ");

  //get some light
  backlightLighten();

  //bling bling
  DoLightsBlingBling();

  //light up
  analogWrite(PIN_LED_WHITE, 255);
  analogWrite(PIN_LED_GREEN, 255);

  boolean vStopSnooze = false;
  while (
  !vStopSnooze
    &&
    millis() < mAlarmRingStart + ALARM_LENGTH 
    ) {
    //loop
    vStopSnooze = PlayMusicString(playlist[random(0, PLAYLIST_LEN)],SOUNDOUT_PIN);

    if (!vStopSnooze) {
      //not touched

      DoLightsBlingBling();
      //light up
      analogWrite(PIN_LED_WHITE, 255);
      analogWrite(PIN_LED_GREEN, 255);

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
  analogWrite(PIN_LED_WHITE, 0);
  analogWrite(PIN_LED_GREEN, 0);

  //store the snooze remaining count
  mSnoozeCountRemaining = max(0, mSnoozeCountRemaining -1);

  //fade to black
  backlightDarken();

  //clear the wake up message
  lcd.clear();
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

void setDateDs1307()                
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.send(0x00);
  Wire.send(decToBcd(0));    //Sec 0 to bit 7 starts the clock
  Wire.send(decToBcd(48)); //min
  Wire.send(decToBcd(19));  //h    // If you want 12 hour am/pm you need to set
  // bit 6 (also need to change readDateDs1307)
  Wire.send(decToBcd(0)); //dow
  Wire.send(decToBcd(27)); //day
  Wire.send(decToBcd(12)); //month
  Wire.send(decToBcd(9)); //year
  Wire.endTransmission();
}

#endif __RTC1307_H__
