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

static void serialHardwareInit(){
	//http://www.avrfreaks.net/forum/tut-soft-using-usart-serial-communications?page=all
	
	UCSRB = (1 << RXEN) | (1 << TXEN); // Turn on the transmission and reception circuitry
	UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1); // Use 8-bit character sizes - URSEL bit set to select the UCRSC register
	
	UBRRH = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
	UBRRL = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register
	
}


