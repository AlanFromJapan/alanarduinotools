//example use of LCD4Bit library
#include <LCD4Bit.h> 
#include <EEPROM.h>

//set debug mode
//#define REVEIL_DEBUG

//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
//Define the I/O pin we will use for our sound output
#define SOUNDOUT_PIN 8
#define WHOLE_NOTE_TIME 1000
//from C1
int mNotes [] = {65,73,82,87,98,110,123,131,147,165,175,196,220,247,262,294,330,349,392,440,494,523,587,659,698,784,880,988,1047,1175,1319,1397,1568,1760,1976,2093,2349,2637,2794,3136,3520,3951,4186,4699 };
#define NOTES_ARRAY_LEN 44
char tididit[] = "e58f58g58  4g58"; //used for saying ok!
//http://gprime.net/images/mariopiano/
char mario3[] = "e48e44e44c48e44g44  2" "c32  8g24  8e22  8" "e24  sa32  sb32  sb34  sa32"
"c34e44g44a54f48g58" "e44c48d48b43" "c43g34  4e22"
;
char clairefont2[] = "g32g34b34" "b34a34b34b34" "g32g34b34" "b34a34b32"
"b32b34a34" "g34b34c44b34" "c42c44b34" "g34b34a32"
"g32g34b34" "b34a38g38b34g34" "b32b34a38g38" "b34a34g32"
;  
char furElise1[] = "e44d44" "e44d44e44b34d44c44" "a32  2a34" "b32  2g34b34" "c42"
;
char nothingElse1[] = "e32g32b32e42b32g32" "e32g32b32e42b32g32" "e32g32b32e42b42" "b54b51b58e41" 
"b54c64b54a54b54a54e41" "b32  8b32e42b32f48  8e42" "e42b32e31"
;
char* playlist[] = {clairefont2, furElise1, nothingElse1, mario3}; 
#define PLAYLIST_LEN 4

//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------


#define PIN_LED_WHITE 10
#define PIN_LED_GREEN 11


#define EEPROM_ADDR_DAYMASK 0
#define EEPROM_ADDR_HOUR 1
#define EEPROM_ADDR_MIN 2
byte mEepromDayMask = 0;
byte mEepromAlarmHour = 0;
byte mEepromAlarmMin = 0;


#include "Wire.h"
#define DS1307_I2C_ADDRESS 0x68  // This is the I2C address


#define PIN_BKL 9
#define BKL_DEFAULT_VAL 0
#define BKL_SHOW_TIME 3000 // how long stay on on touch
boolean mBklIsLightOn = false;
unsigned long mBklStartShowTime = 0;


//modified to match my sparkfun connector
#define xLow  17
#define xHigh 15
#define yLow  16
#define yHigh 14
#define TOUCH_READ_PERIOD 50
#define TIME_READ_TRIGGER (1000 / (TOUCH_READ_PERIOD -1))
byte mReadCount = 0;
boolean mTouching = false;


typedef struct {
  byte second;
  byte minute;
  byte hour;
  byte dayOfWeek;
  byte dayOfMonth; 
  byte month;
  byte year;
} Date;
byte mSnoozeCountRemaining = 0; //number of times snooze should again occure before dropping the alarm
unsigned long mAlarmRingStart = 0; //when the alarm/snooze started
byte mLatestAlarmFinishedDay = -1; //the last day of week the alarm run and stopped (detect todays stop and no reinit at midnight)
#define ALARM_LENGTH 60000 //60 sec
#define ALARM_SNOOZE_COUNT 5 //snooze 5 times before dropping the alarm 
#define ALARM_SNOOZE_DURATION 5 //snooze 5 minutes
#define ALARM_STOP_DURATION 5000 // 5sec to press 
#define ALARM_TOUCHCOUNT_TO_STOP 2 //gotta touch 2 to stop


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


//http://www.uchobby.com/index.php/2007/11/22/arduino-sound-part-3-playing-a-melody/
void Sound(float freq, int durationMS, int outputPin) {
  int halfPeriod;
  float period;
  int durationCycles;
  //Check for rest, 0 frequency is a rest for durationMS.
  if(freq==0.0) {
    //0 frequency so we stay quiet for duration
    delay (durationMS);
  }
  else { //Frequency is not zero so we have work to do
    // turn on output pin
    pinMode(outputPin, OUTPUT);
    //calculate the period or cycle time for the given frequency
    period=1/freq; //Take the reciprocal to get time in seconds
    period=period*1.0E6; //to covert seconds to uS.
    //divide that by 2 to get the 1/2 cycle time. convert to int at the same time
    halfPeriod = (int)(period/2.0) - 7; // subtract 7 us to make up for digitalWrite overhead

    // calculate cycles for duration.
    durationCycles = (int)(((float)durationMS*1000.0)/period); // play note for duration ms
    for (int i=0; i<durationCycles; i++){
      digitalWrite(outputPin, HIGH);
      delayMicroseconds(halfPeriod);
      digitalWrite(outputPin, LOW);
      delayMicroseconds(halfPeriod - 1); // - 1 to make up for fractional microsecond                                              in digitaWrite overhead
    }
    // shut off pin to avoid noise from other operations
    pinMode(outputPin, INPUT);
  }
}



//Returns true if read, and changes values of pX and pY to the touch location
boolean readTouch (int& pX, int& pY){
  pinMode(xLow,OUTPUT);
  pinMode(xHigh,OUTPUT);
  digitalWrite(xLow,LOW);
  digitalWrite(xHigh,HIGH);

  digitalWrite(yLow,LOW);
  digitalWrite(yHigh,LOW);

  pinMode(yLow,INPUT);
  pinMode(yHigh,INPUT);
  delay(10);

  //xLow has analog port -14 !!
  int x=analogRead(yLow -14);
  
  pinMode(yLow,OUTPUT);
  pinMode(yHigh,OUTPUT);
  digitalWrite(yLow,LOW);
  digitalWrite(yHigh,HIGH);

  digitalWrite(xLow,LOW);
  digitalWrite(xHigh,LOW);

  pinMode(xLow,INPUT);
  pinMode(xHigh,INPUT);
  delay(10);

  //xLow has analog port -14 !!
  int y=analogRead(xLow - 14);

  //returns true in case value is over the predefined min level
  if (x > 5 && y > 5){
    //touch, but to avoid erratic rafal touch, ignore if we were already touching
    if (mTouching){
      return false;
    }
    //otherwise return the position and save the fact we touched
    pX = x;
    pY = y;
    mTouching = true;
    return true;
  }
  mTouching = false;
  return false;
}

//http://www.uchobby.com/index.php/2007/11/22/arduino-sound-part-3-playing-a-melody/
//returns true if left by touch, otherwise false
boolean PlayMusicString(char* music, int outputPin){
  int noteCount=strlen(music);
  float freq;
  int duration;
  for (int i=0;i<noteCount;i+=3) {
	if (music[i] == ' '){
		freq=0;
	}
	else {
		byte vNote = 0; //C
		switch(music[i]){
		  case 'c' :
			vNote =0;
			break;
		  case 'd' :
			vNote =1;
			break;
		  case 'e' :
			vNote =2;
			break;
		  case 'f' :
			vNote =3;
			break;
		  case 'g' :
			vNote =4;
			break;
		  case 'a' :
			vNote =5;
			break;
		  case 'b' :
			vNote =6;
			break;	
		}
		byte vOctave = music[i+1] - '1';
		if (vNote + 7 * vOctave < NOTES_ARRAY_LEN){
			freq = mNotes[vNote + 7 * vOctave];
		}
		else {
			//note unknown
			freq = 0;
		}
	}
    //Note Timing
    switch(music[i+2]){
      case '1' :
        duration=WHOLE_NOTE_TIME;
        break;
      case '2' :
        duration=WHOLE_NOTE_TIME/2;
        break;
       case '3' :
        duration=WHOLE_NOTE_TIME/3;
        break;
      case '4' :
        duration=WHOLE_NOTE_TIME/4;
        break;
      case '6' :
        duration=WHOLE_NOTE_TIME/6;
        break;
      case '8' :
        duration=WHOLE_NOTE_TIME/8;
        break;
      case 's' :
        duration=WHOLE_NOTE_TIME/16;
        break;
    }
    Sound(freq,duration,outputPin);
    
    delay(WHOLE_NOTE_TIME/32
      - 30 //estimated time for reading the touch 
    );
    
    //is touched ?
    int x;
    int y;
    if (readTouch(x, y)){
      //leave
      return true;
    }
  }
  return false;
}



// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}

// Gets the date and time from the ds1307 and prints result
char* getDateDs1307(char* pBuff, Date& pD)
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


//does some sort of splash screen
void splashScreen(){
  lcd.clear();
  delay(1000);
  
  //get some light
  backlightLighten();
 
  lcd.printIn("    ardReveil   ");
  lcd.cursorTo(2, 0);
  lcd.printIn("     v 0.2      "); 
  
  delay(1000);
  lcd.leftScroll(20, 50);
  
  //unfortunate compiler optimization forces to manually re-init the char buffer (otherwise buffer overflow)
  //anyway we're lucky, we can use to center the text
  char vTempBuff[5];
  char vBuff[16];
  strcpy(vBuff,"      "); 
  strcat(vBuff, dtostrf(mEepromAlarmHour, 2, 0, vTempBuff));
  strcat(vBuff, ":");
  strcat(vBuff, dtostrf(mEepromAlarmMin, 2, 0, vTempBuff));
  lcd.cursorTo(1, 0);
  lcd.printIn("   Wake-up at   ");
  lcd.cursorTo(2, 0);
  lcd.printIn(vBuff); 
  
  delay(1000);
  lcd.leftScroll(20, 50);
  
  lcd.clear();
    
  //fade to black
  backlightDarken();
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

//reads config saved in the EEPROM
void readEEPROMData(){
  mEepromDayMask = EEPROM.read(EEPROM_ADDR_DAYMASK);
  mEepromAlarmHour = EEPROM.read(EEPROM_ADDR_HOUR);
  mEepromAlarmMin = EEPROM.read(EEPROM_ADDR_MIN);
}

void setup() { 
  /*
  // That arduino pro board is 16 mHz BUT the bootloader is made for 8MHz so force the 
  // Arduino to run half speed (force 8MHz)
  CLKPR = (1<<CLKPCE);
  CLKPR = 1; // Divide by 2 
  */
  
  Wire.begin();
  
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
  
#ifdef REVEIL_DEBUG
  //PlayMusicString(playlist[random(0, 4)],SOUNDOUT_PIN);
  //startAlarm();
  Serial.begin(9600);
 
  
  //reset eeprom settings
  EEPROM.write(EEPROM_ADDR_DAYMASK, 31); //mon-fri
  EEPROM.write(EEPROM_ADDR_HOUR, 07);
  EEPROM.write(EEPROM_ADDR_MIN, 45);
  readEEPROMData();
  
  Serial.println("== EEPROM DATA ==");
  Serial.print(mEepromDayMask,DEC);
  Serial.print(" ");
  Serial.print(mEepromAlarmHour,DEC);
  Serial.print(":");
  Serial.print(mEepromAlarmMin,DEC);
  Serial.print(" ");
  Serial.println("");
#endif
}


void loop() { 
  //read count once per sec at max
  if (mReadCount >= TIME_READ_TRIGGER){
    char vBuff[20];
    Date vD;
    //lcd.clear();
    lcd.cursorTo(1, 0);
    lcd.printIn(getDateDs1307(vBuff, vD));
#ifdef REVEIL_DEBUG
    Serial.print("DoW:");
    Serial.print(vD.dayOfWeek,DEC);   
         
    Serial.println(""); 
#endif        
    mReadCount = 0;
    
    if (checkAlarmRun (vD)){
      //run the alarm
      startAlarm(vD);
    }
    
  }
  
  //turn off light?
  if (mBklIsLightOn && millis() - mBklStartShowTime > BKL_SHOW_TIME){
    mBklIsLightOn = false;
    
    //fade to black
    backlightDarken();
  }
  
  //is touched ?
  int x;
  int y;
  if (readTouch(x, y)){
    //touched!
    if (!mBklIsLightOn){
      //get some light
      backlightLighten();
    }
    //save the info light is on and the show time
    mBklIsLightOn = true;
    mBklStartShowTime = millis();
  }
  
  
  //next round...
  mReadCount += 1;
  delay(TOUCH_READ_PERIOD);
}
