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
#include "USI_I2C/USI_TWI_Master.h"


#define AQTK_I2C_ADDR 0x2E // AquesTalk pico LSI I2C

#define MESSAGEBUF_SIZE       16

void AquestLSI_Init(){
	unsigned char vBuff[MESSAGEBUF_SIZE];
		
	USI_TWI_Master_Initialise();
		
	//heading byte : address + is it read or write
	vBuff[0] = (AQTK_I2C_ADDR<<TWI_ADR_BITS) | (FALSE<<TWI_READ_BIT);
	//from the next byte is the message
	vBuff[1] = '?';
	vBuff[2] = 0;
	vBuff[3] = 13;
	vBuff[4] = 0;
	
	//send 4 bytes
	USI_TWI_Start_Read_Write( vBuff, 5 );
}
	
void SayKonnichiwa(){
	_delay_ms(1000);
	
	char vBuff[MESSAGEBUF_SIZE];
	
	USI_TWI_Master_Initialise();
	

	//heading byte : address + is it read or write
	vBuff[0] = (AQTK_I2C_ADDR<<TWI_ADR_BITS) | (FALSE<<TWI_READ_BIT);
	strncpy(vBuff +1, "konnichiwa.", 11);
	vBuff[1+12] = 0;
	vBuff[1+12+1] = 13;
	vBuff[1+12+1+1] = 0;
	
	//send 4 bytes
	USI_TWI_Start_Read_Write( (unsigned char*) vBuff, 16 );
}