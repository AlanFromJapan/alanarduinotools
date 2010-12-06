#include "WProgram.h"
//#include "wiring.h"


#ifndef __DS3232_H__
#define __DS3232_H__

//if defined, trace things to the serial port
//#define DEBUGSERIAL

#include "Wire.h"

#define DS3232_I2C_ADDRESS B01101000  // This is the I2C address 7bits

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
  
  
  //sprintf(pBuff, "                ", pD.hour, pD.minute, pD.second);
  //sprintf(pBuff, "     %d2:%d2      ", pD.hour, pD.minute, pD.second);
  return pBuff;
}

// Gets the date and time from the ds1307 and prints result
char* getDateDS3232(char* pBuff, Date& pD, int pOffset)
{
  // Reset the register pointer

  Wire.beginTransmission(DS3232_I2C_ADDRESS);
  Wire.send(0x00); //move to reg 0
  Wire.endTransmission();

  int vBytesToRead = 7;
  Wire.requestFrom(DS3232_I2C_ADDRESS, vBytesToRead); 

  // A few of these need masks because certain bits are control bits
  // Beware : the sequence is sligthly different from the RTC DS1307 from Maxim
  pD.second     = bcdToDec(Wire.receive() & 0x7f);
  pD.minute     = bcdToDec(Wire.receive() & 0x7f);
  pD.hour       = bcdToDec(Wire.receive() & 0x3f);  // Need to change this if 12 hour am/pm
  pD.dayOfWeek  = bcdToDec(Wire.receive() & 0x07); //0= sunday
  pD.dayOfMonth = bcdToDec(Wire.receive() & 0x3f);
  pD.month      = bcdToDec(Wire.receive() & 0x1f);
  pD.year       = bcdToDec(Wire.receive());

  //save
  mLatestReadDate = pD;

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
  Serial.println("");
#endif

  Wire.endTransmission();
  
  makeDateStringForLCD(pBuff, pD);
  
  return pBuff;
}

void setDate3232()                
{
  Wire.beginTransmission(DS3232_I2C_ADDRESS);
  Wire.send(0x00);
  Wire.send(decToBcd(00));    // sec
  Wire.send(decToBcd(20));   //min
  Wire.send(decToBcd(22));   //h   
  Wire.send(decToBcd(5)); //dow 0= sunday
  Wire.send(decToBcd(13)); //day
  Wire.send(decToBcd(3)); //month
  Wire.send(decToBcd(10)); //y
  Wire.endTransmission();
}

void setControlRegisters(){
  Wire.beginTransmission(DS3232_I2C_ADDRESS);
  Wire.send(0x0E);  //Goto register 0Eh
  Wire.send(B00011100);
  Wire.send(B10000000);
  Wire.endTransmission();
  
}

void setupRTC3232(){
  //Wire setup to gain access to the RTC
  Wire.begin();
  //setDate3232();
  //setControlRegisters();
}

#endif
