/*
 * TWI.c
 * USe the TWI I2C interface with a ATTiny2313
 * Based on 
 * Created: 2013/01/02 15:36:46
 *  Author: Alan
 */ 

#define F_CPU 1000000UL	      // Sets up the default speed for delay.h
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>
#include <string.h> 
#include <avr/pgmspace.h>
#include "USI_I2C/USI_TWI_Master.h"


#define AQTK_I2C_ADDR 0x2E // AquesTalk pico LSI I2C

#define MESSAGEBUF_SIZE 30
char mBuff[MESSAGEBUF_SIZE];

const char string_0[] PROGMEM = "konnichiwa.";
const char string_1[] PROGMEM = "terebi.";
const char string_2[] PROGMEM = "pasokon.";
const char string_3[] PROGMEM = "arongiwa narongiga suki.";
const char string_4[] PROGMEM = "moichido oshitekudasai.";

const char* const string_table[] PROGMEM = 	   
{
	string_0,
	string_1,
	string_2,
	string_3,
	string_4 
};
uint8_t const string_table_len[]  =
{
	11,
	7,
	8,
	24,
	23
};

void AquestLSI_Init(){		
	USI_TWI_Master_Initialise();
		
	//heading byte : address + is it read or write
	mBuff[0] = (AQTK_I2C_ADDR<<TWI_ADR_BITS) | (FALSE<<TWI_READ_BIT);
	//from the next byte is the message
	mBuff[1] = '?';
	mBuff[2] = 0;
	mBuff[3] = 13; //need the CR char
	mBuff[4] = 0;
	
	//send 4 bytes
	USI_TWI_Start_Read_Write( mBuff, 5 );
}
	
//Returns 1 if busy
//basically recomposed on IsBusy from the AquesTalk's Arduino library
uint8_t LsiIsBusy(){
	//heading byte : address + is it read or write
	mBuff[0] = (AQTK_I2C_ADDR<<TWI_ADR_BITS) | (TRUE<<TWI_READ_BIT);
	mBuff[1] = 0;
	USI_TWI_Start_Read_Write( mBuff, 2 );
	
	//answer is in mBuff[1] !
	
	if (mBuff[1]=='*' || mBuff[1]==0xFF){
		//busy, come back later
		_delay_ms(10);
		return 1;
	}
	return 0;
}

void SayKonnichiwa(){
	_delay_ms(1000);
	
	USI_TWI_Master_Initialise();
	

	//heading byte : address + is it read or write
	mBuff[0] = (AQTK_I2C_ADDR<<TWI_ADR_BITS) | (FALSE<<TWI_READ_BIT);
	strncpy(mBuff +1, "konnichiwa.", 11);
	mBuff[1+12] = 0;
	mBuff[1+12+1] = 13;//need the CR char
	mBuff[1+12+1+1] = 0;
	
	//send 4 bytes
	USI_TWI_Start_Read_Write( (unsigned char*) mBuff, 16 );
}

void SayAll(){
	USI_TWI_Master_Initialise();
		
	for (uint8_t i = 0; i < 5; i++) {
		uint8_t vLen = string_table_len[i];
		
		//heading byte : address + is it read or write
		mBuff[0] = (AQTK_I2C_ADDR<<TWI_ADR_BITS) | (FALSE<<TWI_READ_BIT);
		
		strncpy_P(mBuff+1, (char*)pgm_read_word(&(string_table[i])), vLen); // Necessary casts and dereferencing, just copy.

		mBuff[1+vLen] = 0;
		mBuff[1+vLen+1] = 13;
		mBuff[1+vLen+1+1] = 0;
	
		//send 4 bytes
		USI_TWI_Start_Read_Write( (unsigned char*) mBuff, vLen+3 );
	
		//wait until the LSI is free
		while (LsiIsBusy() == 1){_delay_ms(50);}
	}
}

