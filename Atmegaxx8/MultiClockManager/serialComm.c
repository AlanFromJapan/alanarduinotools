/*
 * serialComm.c
 *
 * Version for ATMEGA328
 *
 * Created: 2015/07/07 23:34:58
 * Updated: 2017/11/01 based on 
          http://maxembedded.com/2013/09/the-usart-of-the-avr/
		  https://hekilledmywire.wordpress.com/2011/01/05/using-the-usartserial-tutorial-part-2/
 *
 *  Author: Alan
 */ 

#include "serialComm.h"

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#include <avr/io.h>
#include <avr/interrupt.h>

void serialHardwareInit(){
	
	UBRR0H = (BAUD_PRESCALE>>8);                      // shift the register right by 8 bits
	UBRR0L = BAUD_PRESCALE;                           // set baud rate
	UCSR0B|= (1<<TXEN0)|(1<<RXEN0);                // enable receiver and transmitter
	UCSR0C|= (1<<UCSZ00)|(1<<UCSZ01);   // 8bit data format

}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

uint8_t USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

//flush the receiving buffer
void USART_Flush( void )
{
	unsigned char dummy;
	while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
}

/*
//Interrupt handler for USART reception
ISR(USART_RXC_vect){
	//http://electronics.stackexchange.com/questions/60759/communicating-via-usart-with-an-avr-input-buffer
	//http://www.fourwalledcubicle.com/AVRArticles.php
	 char ReceivedByte;
	 ReceivedByte = UDR;
	
	//Test echo
	USART_Transmit(ReceivedByte);
}
*/


void USART_SendString (char* pString){
	uint8_t i = 0;
	
	do {
		USART_Transmit(pString[i]);
	} while (pString[i++] != 0);
	
}