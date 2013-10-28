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

#include <avr/io.h>
#include <avr/delay.h>
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
	
int main(void)
{
//	test_leds();

	LcdSetup();
	
	LcdSetPower(1);
	
	//LcdSetInverse(1);
	
	//LcdClear();
	
  for (uint8_t i = 0; i < 8; i++){
	  LcdSetCursor(i,0);
	  LcdSend(PCD8544_DATA, 1 << i);
  }
  for (uint8_t i = 0; i < 8; i++){
	  LcdSetCursor(8+i,1);
	  LcdSend(PCD8544_DATA, 1 << i);
  }
  	
	while(1) {}
}