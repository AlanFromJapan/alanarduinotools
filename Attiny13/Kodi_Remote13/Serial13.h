/*
 * Serial13.h
 *
 * Created: 2016/05/07 18:07:55
 *  Author: Alan
 */ 


#ifndef SERIAL13_H_
#define SERIAL13_H_

#include <avr/io.h>

//If you use internal oscillator at 4.8 MHz, the accuracy is nominal at 3.3V (see doc)
//One bit is 104 uS at 3.3V -> you have to tweak the value if you develop at 5v (at 5v => 114 is good)
#define SER_BIT_US		104

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