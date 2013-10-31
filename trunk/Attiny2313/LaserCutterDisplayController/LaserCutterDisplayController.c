/*
 * LaserCutterDisplay.cpp
 *
 * Created: 2013/10/13 18:55:35
 *  Author: Alan
 */ 

//needed for correct delay.h functions
#define F_CPU 8000000L

#ifndef __AVR_ATtiny2313__
	#define __AVR_ATtiny2313__
#endif

#define I2C_SLAVE_ADDR  0x26            // i2c slave address (38)

#define LCD_COMMAND_CLS		99

#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>

#include "usiTwiSlave.h"
#include "PCD8544.h"



void test_leds(){
	DDRA = 0x03;
	
	uint8_t v = 0;
	while (1){
		PORTA = (1 << v);
		
		_delay_ms(1000);
		
		v += 1;
		v = v % 2;
	}	
	
}

void setupTwiSlave() {
	//PA0 and PA1 are output, the rest is input
	DDRA = 0x03;
	
	//pullups on TWI ports
	DDRB = ~((1 << 7) | (1 << 5));
	PORTB = (1 << 7) | (1 << 5);
	
	usiTwiSlaveInit(I2C_SLAVE_ADDR);
	
	// Enable global interrupts (required for I2C)
	sei();	
}

void testTwiLoop(){
	while(1)
	{
		uint8_t vReceived = 0;
		if (usiTwiDataInReceiveBuffer()){
			//data are ready
			vReceived = usiTwiReceiveByte();
			
			//led on
			PORTA = (1 << vReceived);
		}
		
		//_delay_ms(100);
	}	
}

void testTwiString(){
	uint8_t vBuff[LCD_MAXCHAR_PER_LINE];
	LcdSetCursor(0,0);
	
	while(1)
	{
		uint8_t vReceived = 0;
		uint8_t vPos = 0;
		
		if (usiTwiDataInReceiveBuffer()){
			vReceived = 0xff;
			
			do {
				if (usiTwiDataInReceiveBuffer()){
					//data are ready
					vReceived = usiTwiReceiveByte();
					vBuff[vPos] = vReceived;
					vPos++;
				}
			} while (vReceived != 0 && vPos < LCD_MAXCHAR_PER_LINE);
			
			LcdClear();
			LcdWrite(vBuff);
		}
		
		//_delay_ms(100);
	}
}


static uint8_t mBuff[LCD_MAXCHAR_PER_LINE+1];

void loopTwiString(){
	LcdSetCursor(0,0);
	
	while(1)
	{
		uint8_t vReceived = 0;
		uint8_t vPos = 0;
		//message format is 1 byte (line), the rest is message
		if (usiTwiDataInReceiveBuffer()){
			vReceived = usiTwiReceiveByte();
			
			if (vReceived == LCD_COMMAND_CLS){
				//clear screen
				LcdClear();
			}
			else {
				//other, write text
				LcdSetCursor(0, vReceived);
								
				//force the last value to not be 0 for the do while
				vReceived = 0xff;
				vPos = 0;
				
				do {
					if (usiTwiDataInReceiveBuffer()){
						//data are ready
						vReceived = usiTwiReceiveByte();
						mBuff[vPos] = vReceived;
						vPos++;
					}
				} while (vReceived != 0 && vPos < LCD_MAXCHAR_PER_LINE);
			
		
				//just in case force a null terminated string
				mBuff[LCD_MAXCHAR_PER_LINE] = 0;
				LcdWrite(mBuff);
			}			
		}
		
	}
}
	
	/*
void testLines(){
	LcdSetCursor(0,0);
	
	LcdSetCursor(0, 0);
	strncpy(mBuff, "******zero", LCD_MAXCHAR_PER_LINE);
	mBuff[LCD_MAXCHAR_PER_LINE] = 0;
	LcdWrite(mBuff);


	LcdSetCursor(0, 1);
	strncpy(mBuff, " un", LCD_MAXCHAR_PER_LINE);
	mBuff[LCD_MAXCHAR_PER_LINE] = 0;
	LcdWrite(mBuff);


	LcdSetCursor(0, 2);
	strncpy(mBuff, "    deux", LCD_MAXCHAR_PER_LINE);
	mBuff[LCD_MAXCHAR_PER_LINE] = 0;
	LcdWrite(mBuff);


	LcdSetCursor(0, 3);
	strncpy(mBuff, "   trois", LCD_MAXCHAR_PER_LINE);
	mBuff[LCD_MAXCHAR_PER_LINE] = 0;
	LcdWrite(mBuff);

//
	//LcdSetCursor(0, 4);
	//strncpy(mBuff, "quatre*", LCD_MAXCHAR_PER_LINE);
	//mBuff[LCD_MAXCHAR_PER_LINE] = 0;
	//LcdWrite(mBuff);


	LcdSetCursor(0, 5);
	strncpy(mBuff, "  cinq", LCD_MAXCHAR_PER_LINE);
	mBuff[LCD_MAXCHAR_PER_LINE] = 0;
	LcdWrite(mBuff);


}
		*/
int main(void)
{
	setupTwiSlave();
	
	LcdSetup();
	LcdSetPower(1);
	
	LcdClear();
	
	  
	loopTwiString();	
}