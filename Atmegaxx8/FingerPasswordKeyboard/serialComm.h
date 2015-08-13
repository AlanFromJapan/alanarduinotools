/*
 * serialComm.h
 *
 * Created: 2015/07/07 23:38:08
 *  Author: Alan
 */ 


#ifndef SERIALCOMM_H_
#define SERIALCOMM_H_


void serialHardwareInit();
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );
void USART_Flush( void );

#endif /* SERIALCOMM_H_ */