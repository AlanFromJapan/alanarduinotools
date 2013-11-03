#include "Arduino.h"

#ifndef __LCDDISPLAY_H__
#define __LCDDISPLAY_H__

#define LCD_I2C_ADDRESS   0x26
#define LCD_COMMAND_CLS   99

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

void lcdWriteLine (uint8_t pLine, uint8_t* pText){
  Wire.beginTransmission(LCD_I2C_ADDRESS);
  Wire.write(pLine);
  Wire.write(pText);
  Wire.write(0);
  delay(LCD_I2C_TEMPORISATION);
  Wire.endTransmission();
  delay(LCD_I2C_TEMPORISATION);
}

void lcdShowStartupScreen(){
  lcdClear();
  
  Wire.beginTransmission(LCD_I2C_ADDRESS);
  Wire.write(1);Wire.write("  * Startup *");Wire.write(0);delay(LCD_I2C_TEMPORISATION);
  Wire.endTransmission();delay(LCD_I2C_TEMPORISATION);Wire.beginTransmission(LCD_I2C_ADDRESS);
  Wire.write(3);Wire.write(" Laser cutter");Wire.write(0);delay(LCD_I2C_TEMPORISATION);
  Wire.endTransmission();delay(LCD_I2C_TEMPORISATION);Wire.beginTransmission(LCD_I2C_ADDRESS);
  Wire.write(4);Wire.write("     by");Wire.write(0);delay(LCD_I2C_TEMPORISATION);
  Wire.endTransmission();delay(LCD_I2C_TEMPORISATION);Wire.beginTransmission(LCD_I2C_ADDRESS);
  Wire.write(5);Wire.write("AlanFromJapan");Wire.write(0);delay(LCD_I2C_TEMPORISATION);
  Wire.endTransmission();
}

void lcdShowPositionAdjustScreen(){
  lcdClear();
  
  Wire.beginTransmission(LCD_I2C_ADDRESS);
  Wire.write(1);Wire.write("  * Adjust *");Wire.write(0);delay(LCD_I2C_TEMPORISATION);
  Wire.endTransmission();delay(LCD_I2C_TEMPORISATION);Wire.beginTransmission(LCD_I2C_ADDRESS);
  Wire.write(3);Wire.write(" Move laser");Wire.write(0);delay(LCD_I2C_TEMPORISATION);
  Wire.endTransmission();delay(LCD_I2C_TEMPORISATION);Wire.beginTransmission(LCD_I2C_ADDRESS);
  Wire.write(4);Wire.write(" head using");Wire.write(0);delay(LCD_I2C_TEMPORISATION);
  Wire.endTransmission();delay(LCD_I2C_TEMPORISATION);Wire.beginTransmission(LCD_I2C_ADDRESS);
  Wire.write(5);Wire.write(" joystick.");Wire.write(0);delay(LCD_I2C_TEMPORISATION);
  Wire.endTransmission();
}

void setupLcdDisplay(){
  Wire.begin(); // join i2c bus (address optional for master)
  //a little delay to give change to the next I2C calls works fine
  delay(50);
  
  lcdClear();
}

#endif //__LCDDISPLAY_H__
