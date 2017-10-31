/*
 * DS3231.h
 *
 * Created: 2017/10/30 23:22:48
 *  Author: Alan
 
  DataSheet: https://datasheets.maximintegrated.com/en/ds/DS3231.pdf
  TWI sample : http://www.embedds.com/programming-avr-i2c-interface/
  util/TWI.h : D:\arduino-1.6.13\hardware\tools\avr\avr\include\util
  
 */ 


#ifndef DS3231_H_
#define DS3231_H_

#include "MCMShared.h"
#include "twi.h"

//for the TWI constants 
#include <util/twi.h>

#define RTC_DS3231

#ifdef RTC_DS3231
	#define RTC_READ_TIME(p)	ReadTime3231(p)
	#define RTC_INIT()			setupDS3231()
	#define RTC_SET_TIME(p)		SetTimeDate3231(p)
#endif


#define DS3231_I2C_ADDRESS 0x68 // B01101000  // This is the I2C address 7bits


void ReadTime3231(Date* pTimeDate){
	uint8_t v = 0x00;
	
	TWIStart();
	if (TWIGetStatus() != TW_START){
		//something wrong
		pTimeDate->hour = 99;
		pTimeDate->minute = pTimeDate->hour;
		pTimeDate->second = pTimeDate->minute;	
		return;
	}
	
	TWIWrite((DS3231_I2C_ADDRESS << 1) | TW_WRITE);
	TWIWrite(0x00); //move to reg 0
	TWIStop();
	
	_delay_ms(20); //in case
	
	TWIStart();
	TWIWrite((DS3231_I2C_ADDRESS << 1) | TW_READ);
	if (TWIGetStatus() != TW_MR_SLA_ACK){
		//something wrong
		pTimeDate->hour = 98;
		pTimeDate->minute = pTimeDate->hour;
		pTimeDate->second = pTimeDate->minute;
		return;
	}
	
	//Read 1: Seconds
	v = TWIReadNACK();	
	if (TWIGetStatus() != TW_MR_DATA_NACK){
		//something wrong
		pTimeDate->hour = 97;
		pTimeDate->minute = pTimeDate->hour;
		pTimeDate->second = pTimeDate->minute;
		return;
	}
	pTimeDate->second = bcdToDec(v & 0x7f);
	
	//...etc etc...
	
	//Finished the read phase
	TWIStop();
	
}
uint8_t SetTimeDate3231(Date* pDateTime){ return 0; }	
uint8_t setupDS3231(){ return 0; }
	
/*
// Gets the Date3231 and time from the ds1307 and prints result
void getDateDS3231(Date& pD)
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

  Wire.endTransmission();
}

void setDateDS3231()                
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write((uint8_t)0x00);
  Wire.write(decToBcd(00));    // sec
  Wire.write(decToBcd(27));   //min
  Wire.write(decToBcd(22));   //h   
  Wire.write(decToBcd(1)); //dow 0= sunday
  Wire.write(decToBcd(27)); //day
  Wire.write(decToBcd(5)); //month
  Wire.write(decToBcd(13)); //y
  Wire.endTransmission();
}

void setDateDS3231(Date& pDate)                
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write((uint8_t)0x00);
  Wire.write(decToBcd(pDate.second));    // sec
  Wire.write(decToBcd(pDate.minute));   //min
  Wire.write(decToBcd(pDate.hour));   //h   
  Wire.write(decToBcd(pDate.dayOfWeek)); //dow 0= sunday
  Wire.write(decToBcd(pDate.dayOfMonth)); //day
  Wire.write(decToBcd(pDate.month)); //month
  Wire.write(decToBcd(pDate.year)); //y
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
*/

#endif /* DS3231_H_ */