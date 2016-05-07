#include "Serial13.h"
#include <avr/io.h>
#include <avr/delay.h>


/************************************************************************/
/* Sets the DIRECTION register for PB3                                  */
/************************************************************************/
void setupSerial(){
	//DO NOT FORGET this, otherwise your SERIAL output will be ugly (not square signal)
	//Set SERIAL pin mode as OUTPUT, set others as INPUT
	//DDRB =  (1 << SET_TX_PIN);
	SER_DIRECT =  (1 << SET_TX_PIN);
	
}
	
/************************************************************************/
/* Bitbanging of serial protocol (SENDING only - 9600/8N1)              */
/************************************************************************/
void serialSendChar (uint8_t c){
	//line go down (start)
	SER_PORT = SER_PORT & ~(1 << SET_TX_PIN);
	_delay_us(SER_BIT_US);
	
	////send bit per bit (MSB first)
	//for (uint8_t i = 0; i < 8; i++){
		//uint8_t d = c & 0x80;
		//
		//if (d == 0){
			//SER_PORT = SER_PORT & ~(1 << SET_TX_PIN);
		//}
		//else {
			//SER_PORT = SER_PORT | (1 << SET_TX_PIN);
		//}
		////wait
		//_delay_us(SER_BIT_US);
		//
		////next
		//c = c << 1;
	//}


	//send bit per bit (LSB first)
	for (uint8_t i = 0; i < 8; i++){
		if ((c & 0x01) == 0){
			SER_PORT = SER_PORT & ~(1 << SET_TX_PIN);
		}
		else {
			SER_PORT = SER_PORT | (1 << SET_TX_PIN);
		}
		//wait
		_delay_us(SER_BIT_US);
		
		//next
		c = c >> 1;
	}

	//line goes up 
	SER_PORT = SER_PORT | (1 << SET_TX_PIN);
	//stop bit
	_delay_us(SER_BIT_US);

}


void __sendHex(uint8_t v){
	if (v < 10)
	serialSendChar('0' + v);
	else
	serialSendChar('A' + v - 10);
}

void serialSendHex(uint8_t v){
	__sendHex(v >> 4);
	__sendHex(v & 0x0f);
}


void serialSendString(char* pStr){
	while (*pStr != 0){
		serialSendChar(*pStr);
		pStr += 1;
	}
}