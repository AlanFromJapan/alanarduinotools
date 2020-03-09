/*
 * serial_utils.c
 *
 *  Created on: Mar 9, 2020
 *      Author: alan
 */


#include <avr/io.h>
#include <util/delay.h>
//#include <avr/interrupt.h>
#include <stddef.h>
#include <stdlib.h>

#include "serial_utils.h"


void delayLong()
{
	unsigned int delayvar;
	delayvar = 0;
	while (delayvar <=  65500U)
	{
		asm("nop");
		delayvar++;
	}
}


inline unsigned char serialCheckRxComplete(void)
{
	return( UCSR0A & _BV(RXC0)) ;		// nonzero if serial data is available to read.
}

unsigned char serialCheckTxReady(void)
{
	return( UCSR0A & _BV(UDRE0) ) ;		// nonzero if transmit register is ready to receive new data.
}

unsigned char serialRead(void)
{
	while (serialCheckRxComplete() == 0)		// While data is NOT available to read
{;;}
	return UDR0;
}

void serialWrite(unsigned char DataOut)
{
	while (serialCheckTxReady() == 0)		// while NOT ready to transmit
{;;}
	UDR0 = DataOut;
}

void serialWriteString(const char* pString){
	if (pString == NULL)
		return;

	uint8_t i, c;
	i =0;
	while ((c = pString[i]) != 0){
		i++;
		serialWrite(c);
	}
}

void establishContact() {
	while (serialCheckRxComplete() == 0) {
		//serialWrite('A');
		delayLong();
	}
#ifdef TALKATIVE
	serialWriteString("***GBCartridgeKS ***\r\n");
#endif //TALKATIVE
}


void setupSerial() {
	//Thanks to http://www.evilmadscientist.com/2009/basics-serial-communication-with-avr-microcontrollers/
	//Copy paste, worked from the first try

	//Serial Initialization

	/*Set baud rate */
	UBRR0H = (unsigned char)(MYUBRR>>8);
	UBRR0L = (unsigned char) MYUBRR;
	/* Enable receiver and transmitter   */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Frame format: 8data, No parity, 1stop bit */
	UCSR0C = (3<<UCSZ00);
}

//////////////////////////////////////////////////////////////////////////
// Works for [0..9A-F]
uint8_t hexToInt(uint8_t pChar){
	//if lowcap make CAPS
	if (pChar >= ASCIIa){
		pChar -= 32;
	}

	if (pChar >= ASCII0 && pChar<= ASCII9)	{
		return pChar - ASCII0;
	}
	else {
		return 10+ pChar - ASCIIA;
	}
}

