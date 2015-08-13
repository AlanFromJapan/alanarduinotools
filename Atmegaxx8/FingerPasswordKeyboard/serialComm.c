/*
 * serialComm.c
 *
 * Created: 2015/07/07 23:34:58
 *  Author: Alan
 */ 

#include "serialComm.h"

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#include <avr/io.h>
#include <avr/interrupt.h>

void serialHardwareInit(){
	//http://www.avrfreaks.net/forum/tut-soft-using-usart-serial-communications?page=all
	
	UCSRB = (1 << RXEN) | (1 << TXEN); // Turn on the transmission and reception circuitry
	UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1); // Use 8-bit character sizes - URSEL bit set to select the UCRSC register
	
	UBRRH = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
	UBRRL = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register
	
	//enable interrupt on receive char (USART_RXC_vect)
	//UCSRB |= (1 << RXCIE );
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) )
	;
	/* Put data into buffer, sends the data */
	UDR = data;
}

uint8_t USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) )
	;
	/* Get and return received data from buffer */
	return UDR;
}

//flush the receiving buffer
void USART_Flush( void )
{
	unsigned char dummy;
	while ( UCSRA & (1<<RXC) ) dummy = UDR;
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