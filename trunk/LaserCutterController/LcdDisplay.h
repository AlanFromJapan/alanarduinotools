#include "Arduino.h"

#ifndef __LCDDISPLAY_H__
#define __LCDDISPLAY_H__

#define LCD_I2C_ADDRESS   0x26
#define LCD_COMMAND_CLS   99
#define LCD_COMMAND_COLOR 98

#define LCD_COLOR_RED 0x01
#define LCD_COLOR_BLUE 0x02
#define LCD_COLOR_PURPLE 0x03
#define LCD_COLOR_OFF 0x00

#define LCD_I2C_TEMPORISATION 10

/***************************
**
** Format is 1st byte is line number [0:5], next is message (max 14 chars) + \0 char
**
***************************/

void lcdClear(){
  Wire.beginTransmission(LCD_I2C_ADDRESS);
  Wire.write(LCD_COMMAND_CLS);delay(LCD_I2C_TEMPORISATION);
  Wire.endTransmission();
  delay(LCD_I2C_TEMPORISATION);
}

void lcdWriteLine (const uint8_t pLine, const char* pText){
  Wire.beginTransmission(LCD_I2C_ADDRESS);
  Wire.write(pLine);
  Wire.write(pText);
  Wire.write(0);
  delay(LCD_I2C_TEMPORISATION);
  Wire.endTransmission();
  delay(LCD_I2C_TEMPORISATION);
}

void lcdSetColor (const uint8_t pColor){
  Wire.beginTransmission(LCD_I2C_ADDRESS);
  Wire.write(LCD_COMMAND_COLOR);
  Wire.write(pColor);
  delay(LCD_I2C_TEMPORISATION);
  Wire.endTransmission();
  delay(LCD_I2C_TEMPORISATION);
}

void lcdShowStartupScreen2(){
  lcdClear();
  
  lcdWriteLine(0, "  * Startup *");
  lcdWriteLine(2, "Resetting pos");
  lcdWriteLine(3, "of bed and");
  lcdWriteLine(4, "head.");
  //lcdWriteLine(5, "Please wait.");
}

void lcdShowStartupScreen(){
  lcdClear();
  
  lcdWriteLine(0, "  * Startup *");
  lcdWriteLine(3, " Laser cutter");
  lcdWriteLine(4, "     by");
  lcdWriteLine(5, "AlanFromJapan");
}

void lcdShowPositionAdjustScreen(){
  lcdClear();
  
  lcdWriteLine(1, "  * Adjust *");
  lcdWriteLine(3, " Move laser");
  lcdWriteLine(4, " head using");
  lcdWriteLine(5, " joystick.");
}

void setupLcdDisplay(){
  Wire.begin(); // join i2c bus (address optional for master)
  //a little delay to give change to the next I2C calls works fine
  delay(50);
  
  lcdClear();
}

#endif //__LCDDISPLAY_H__
