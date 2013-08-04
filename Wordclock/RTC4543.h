#include "Arduino.h"

#ifndef __RTC4543_h__
#define __RTC4543_h__

#include "WordclockShared.h"
#include <util/delay.h>

#define RTC4543_PIN_CE 		2
#define RTC4543_PIN_WR 		3
#define RTC4543_PIN_DATA 	4
#define RTC4543_PIN_CLOCK	5

void setupRTC4543 (){
	//make the 3 pins output
	DDRC |= (1 << RTC4543_PIN_CE) | (1 << RTC4543_PIN_WR) | (1 << RTC4543_PIN_CLOCK);
}


//Assumes start with CLOCK=low, after startup sequence
//First bit of data is ready to be read
uint8_t returnByte (uint8_t pBitCount){
	uint8_t v = 0;
	for (uint8_t i = 0; i < pBitCount; i++){
		if ((PINC & (1 << RTC4543_PIN_DATA)) != 0){
			v |=(1 << i);
		}
		
		//CLOCK goes up to ask for next bit
		PORTC |= (1 << RTC4543_PIN_CLOCK);
		//lCLKH
		_delay_us(1);
		//CLOCK goes low
		PORTC &= ~(1 << RTC4543_PIN_CLOCK);
		//lCLKL (not mandatory I think)
		_delay_us(1);		
	}
	return v;
}

//Assumes start with CLOCK=low, after startup sequence
void writeByte (uint8_t pByte, uint8_t pBitCount){
	for (uint8_t i = 0; i < pBitCount; i++){
                if (((pByte >> i) & 0x01) != 0){
                    PORTC |= (1 << RTC4543_PIN_DATA);
                }
                else {
                    PORTC &= ~(1 << RTC4543_PIN_DATA);
                }
		
		//CLOCK goes up to tell the RTC to read
		PORTC |= (1 << RTC4543_PIN_CLOCK);
		//lCLKH
		_delay_us(1);
		//CLOCK goes low
		PORTC &= ~(1 << RTC4543_PIN_CLOCK);
		//lCLKL (not mandatory I think)
		_delay_us(1);		
	}
}

void ReadTime4543(Date& TimeDate){
   //int TimeDate [7]; //second,minute,hour,null,day,month,year		
	uint8_t vBCD = 0;
	uint8_t i = 0;

	//Set data as input
	DDRC &= ~(1 << RTC4543_PIN_DATA);
	
	//Startup sequence
	//Clock go low then WR
	PORTC &= ~(1 << RTC4543_PIN_CLOCK);
	PORTC &= ~(1 << RTC4543_PIN_WR);
	// 25ns <= tCLKS <= 50 ns : give it 1 us
	_delay_us(1);
	// CE goes up
	PORTC |= (1 << RTC4543_PIN_CE);
	// wait tCES
	_delay_us(1);

        //first clock hedge just LOADED the data, so just discard it (or everything will be 1 bit shifted)
        returnByte(1);
        
	//Second
	vBCD = returnByte(8);
	TimeDate.second = bcdToDec(vBCD);
	//Minutes
	vBCD = returnByte(8);
	vBCD = vBCD & 0x7f;
	TimeDate.minute = bcdToDec(vBCD);
	//Hours
	vBCD = returnByte(8);
	vBCD = vBCD & 0x3f;
	TimeDate.hour = bcdToDec(vBCD);
	//Day of week (skip)
	vBCD = returnByte(4);
	i++;
	//Day of month
	vBCD = returnByte(8);
	vBCD = vBCD & 0x3f;
	TimeDate.dayOfMonth = bcdToDec(vBCD);
	//Month
	vBCD = returnByte(8);
	vBCD = vBCD & 0x1f;
	TimeDate.month = bcdToDec(vBCD);
	//Year
	vBCD = returnByte(8);
	TimeDate.year = bcdToDec(vBCD);
	
	//Outro 
	//WR goes HIGH, CE & CLOCK low
	PORTC &= ~(1 << RTC4543_PIN_CLOCK);
	PORTC &= ~(1 << RTC4543_PIN_CE);
	PORTC |= (1 << RTC4543_PIN_WR); 
}


void WriteTime4543(Date& TimeDate){
   //int TimeDate [7]; //second,minute,hour,null,day,month,year		
	//Set data as output
	DDRC |= (1 << RTC4543_PIN_DATA);
	
	//Startup sequence
	//Clock go low then WR (down then up to make sure we have a upgoing front)
	PORTC &= ~(1 << RTC4543_PIN_WR);
	_delay_us(1);
	PORTC &= ~(1 << RTC4543_PIN_CLOCK);
	PORTC |= (1 << RTC4543_PIN_WR);
	// 25ns <= tCLKS <= 50 ns : give it 1 us
	_delay_us(1);
	// CE goes up
	PORTC |= (1 << RTC4543_PIN_CE);
	// wait tCES
	_delay_us(1);

	//Second
	writeByte(decToBcd(TimeDate.second), 8);
	//Minutes
	writeByte(decToBcd(TimeDate.minute) & 0x7f, 8);
	//Hours
	writeByte(decToBcd(TimeDate.hour) & 0x3f, 8);
	//Day of week (skip)
	writeByte(0x00, 4);
	//Day of month
	writeByte(decToBcd(TimeDate.dayOfMonth) & 0x3f, 8);
	//Month
	writeByte(decToBcd(TimeDate.month) & 0x1f, 8);
	//Year
	writeByte(decToBcd(TimeDate.year), 8);

	//Outro 
	//WR goes LOW, CE & CLOCK low
	PORTC &= ~(1 << RTC4543_PIN_CLOCK);
	PORTC &= ~(1 << RTC4543_PIN_CE);
	PORTC &= ~(1 << RTC4543_PIN_WR);
}


void setDate4543()                
{
  Date d;
  d.second = 0;
  d.minute = 48;
  d.hour = 22;
  d.dayOfMonth = 4;
  d.month = 8;
  d.year = 13;
  
  WriteTime4543(d);
}

#endif //__RTC4543_h__
