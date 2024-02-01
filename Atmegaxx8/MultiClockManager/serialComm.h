/*
 * serialComm.h
 *
 * Created: 2015/07/07 23:38:08
 *  Author: Alan
 *
 *  2024/01/14 For future me: the USART pins RX/TX are PD0/PD1. Don't thank me.
 *
 */ 


#ifndef SERIALCOMM_H_
#define SERIALCOMM_H_

void USART_SendString (char* pString);
void serialHardwareInit();
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );
void USART_Flush( void );

#endif /* SERIALCOMM_H_ */
