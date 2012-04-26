#include "Arduino.h"


#ifndef __DS3231_H__
#define __DS3231_H__

//if defined, trace things to the serial port
//#define DEBUGSERIAL

#include "Wire.h"

#define DS3231_I2C_ADDRESS B01101000  // This is the I2C address 7bits

typedef struct {
  byte second;
  byte minute;
  byte hour;
  byte dayOfWeek;
  byte dayOfMonth; 
  byte month;
  byte year;
} 
Date3231;

Date3231 mLatestReadDate;

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




// Gets the Date3231 and time from the ds1307 and prints result
void getDateDS3231(Date3231& pD)
{
  // Reset the register pointer

  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write((uint8_t)0x00); //move to reg 0
  Wire.endTransmission();

  int vBytesToRead = 7;
  Wire.requestFrom(DS3231_I2C_ADDRESS, vBytesToRead); 

  // A few of these need masks because certain bits are control bits
  // Beware : the sequence is sligthly different from the RTC DS1307 from Maxim
  pD.second     = bcdToDec(Wire.read() & 0x7f);
  pD.minute     = bcdToDec(Wire.read() & 0x7f);
  pD.hour       = bcdToDec(Wire.read() & 0x3f);  // Need to change this if 12 hour am/pm
  pD.dayOfWeek  = bcdToDec(Wire.read() & 0x07); //0= sunday
  pD.dayOfMonth = bcdToDec(Wire.read() & 0x3f);
  pD.month      = bcdToDec(Wire.read() & 0x1f);
  pD.year       = bcdToDec(Wire.read());

  //save
  mLatestReadDate = pD;

  Wire.endTransmission();
}

void setDateDS3231()                
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write((uint8_t)0x00);
  Wire.write(decToBcd(00));    // sec
  Wire.write(decToBcd(14));   //min
  Wire.write(decToBcd(23));   //h   
  Wire.write(decToBcd(2)); //dow 0= sunday
  Wire.write(decToBcd(28)); //day
  Wire.write(decToBcd(2)); //month
  Wire.write(decToBcd(12)); //y
  Wire.endTransmission();
}

void setDateDS3231(Date3231 pDate)                
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write((uint8_t)0x00);
  Wire.write(decToBcd(pDate.second));    // sec
  Wire.write(decToBcd(pDate.minute));   //min
  Wire.write(decToBcd(pDate.hour));   //h   
  Wire.write(decToBcd(2)); //dow 0= sunday
  Wire.write(decToBcd(28)); //day
  Wire.write(decToBcd(2)); //month
  Wire.write(decToBcd(12)); //y
  Wire.endTransmission();
}

void setControlRegisters(){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0x0E);  //Goto register 0Eh
  Wire.write(B00011100);
  Wire.write(B10000000);
  Wire.endTransmission();
  
}

int setupDS3231(boolean pSetRegisters){
  //Wire setup to gain access to the RTC
  Wire.begin();
  //setDate3232();
  setControlRegisters();
  
  return 0;
}

int setupDS3231(){
  return setupDS3231(false);
}

#endif
