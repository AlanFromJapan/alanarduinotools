//example use of LCD4Bit library
#include <LCD4Bit.h> 

#include "Wire.h"
#define DS1307_I2C_ADDRESS 0x68  // This is the I2C address

#define PIN_BKL 9
#define BKL_DEFAULT_VAL 0

//create object to control an LCD.  
//number of lines in display=1
LCD4Bit lcd = LCD4Bit(2); 

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}

// Gets the date and time from the ds1307 and prints result
char* getDateDs1307(char* pBuff)
{
  // Reset the register pointer
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.send(0x00);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);

  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // A few of these need masks because certain bits are control bits
  second     = bcdToDec(Wire.receive() & 0x7f);
  minute     = bcdToDec(Wire.receive());
  hour       = bcdToDec(Wire.receive() & 0x3f);  // Need to change this if 12 hour am/pm
  dayOfWeek  = bcdToDec(Wire.receive());
  dayOfMonth = bcdToDec(Wire.receive());
  month      = bcdToDec(Wire.receive());
  year       = bcdToDec(Wire.receive());
  
  char vTempBuff[5];
  //unfortunate compiler optimization forces to manually re-init the char buffer (otherwise buffer overflow)
  //anyway we're lucky, we can use to center the text
  strcpy(pBuff,"    "); 
  strcat(pBuff, dtostrf(hour, 2, 0, vTempBuff));
  strcat(pBuff, ":");
  strcat(pBuff, dtostrf(minute, 2, 0, vTempBuff));
  strcat(pBuff, ":");
  strcat(pBuff, dtostrf(second, 2, 0, vTempBuff));

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
  lcd.printIn("     v 0.1      "); 
  
  delay(1000);
  lcd.leftScroll(20, 50);
  
  lcd.clear();
    
  //fade to black
  backlightDarken();
}

void setup() { 
  // That arduino pro board is 16 mHz BUT the bootloader is made for 8MHz so force the 
  // Arduino to run half speed (force 8MHz)
  CLKPR = (1<<CLKPCE);
  CLKPR = 1; // Divide by 2 

  Wire.begin();
  
  lcd.init();
  lcd.clear();
  
  //reset to default
  analogWrite(PIN_BKL, BKL_DEFAULT_VAL);
  
  splashScreen();
  
  //optionally, now set up our application-specific display settings, overriding whatever the lcd did in lcd.init()
  //lcd.commandWrite(0x0F);//cursor on, display on, blink on.  (nasty!)
}

void loop() {  
  char vBuff[20];
  //lcd.clear();
  lcd.cursorTo(1, 0);
  lcd.printIn(getDateDs1307(vBuff));
  delay(1000);
}
