/*
 * Serial13.h
 *
 * Created: 2016/05/07 18:07:55
 *  Author: Alan
 */ 


#ifndef SERIAL13_H_
#define SERIAL13_H_

#include <avr/io.h>

//One bit is 104 uS -> You have to tweak this value to match your crystal OR get a precise crystal.
//In my case, I use the infamous internal crystal of my ATtiny 13 which is 9.6% off !!
#define SER_BIT_US		114

#define SER_PORT		PORTB
#define SER_DIRECT		DDRB
#define SET_TX_PIN		PB3


/************************************************************************/
/* Sets the DIRECTION register for PB3                                  */
/************************************************************************/
void setupSerial();

/************************************************************************/
/* Bitbanging of serial protocol (SENDING only - 9600/8N1)              */
/************************************************************************/
void serialSendChar (uint8_t c);

void serialSendHex(uint8_t v);

void serialSendString(char* pStr);

#endif /* SERIAL13_H_ */