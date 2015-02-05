/*
 * GbCartridgeKSController.cpp
 *
 * Created: 2015/02/05 23:35:27
 *  Author: Alan
 */ 


#include <avr/io.h>
//#include <avr/interrupt.h>
#include <stddef.h>
#include <stdlib.h>

#define F_CPU 16000000	// 16 MHz oscillator.
#define BaudRate 9600
#define MYUBRR (F_CPU / 16 / BaudRate ) - 1

#define TALKATIVE 

#define DATABUS_WRITE_MODE { DDRA = 0xFF; }
#define DATABUS_READ_MODE { DDRA = 0x00; }

#define ASCII0  48
#define ASCII9  57
#define ASCIIA  65

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
		delayLong();
		delayLong();
		delayLong();
		delayLong();
		delayLong();
		delayLong();
	}
#ifdef TALKATIVE	
	serialWriteString("***GBCartridgeKS ***\r\n");
#endif //TALKATIVE
}


void setupSerial() 
{
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


void setupExternalFlash(){
	//address 0..7
	DDRF = 0xFF;
	//address 8..15
	DDRC = 0xFF;
	//address 16..17
	DDRE |= 0xC0; //sets PE7 and PE6
	
	//commands PB3..PB6
	DDRB |= 0x78;
	
	//data bus bi-directional so will change
	DATABUS_WRITE_MODE;
}

//////////////////////////////////////////////////////////////////////////
// Works for [0..9A-F]
uint8_t hexToInt(uint8_t pChar){
	if (pChar >= ASCII0 && pChar<= ASCII9)	{
		return pChar - ASCII0;
	}
	else {
		return pChar - ASCIIA;		
	}
}

uint8_t flashGetByteDecode() {
	uint8_t inByte = 0;         // incoming serial byte

	
	//read the 3 address byte MSB first
	inByte = serialRead() - ASCII0;
	PORTE = (inByte << 6) | (PORTE & 0x3F);

	inByte = hexToInt(serialRead());
	inByte = inByte << 4;
	inByte = inByte | hexToInt(serialRead());
	PORTC = inByte;

	inByte = hexToInt(serialRead());
	inByte = inByte << 4;
	inByte = inByte + hexToInt(serialRead());
	PORTF = inByte;


#ifdef TALKATIVE

serialWriteString("GET addr: ");

char vBuffChar[4];
serialWriteString(itoa(PORTC, vBuffChar, 16));

serialWriteString(" ");
serialWriteString(itoa(PORTF, vBuffChar, 16));

serialWriteString("\r\n");
#endif //TALKATIVE	
		
}

void decodeCommand (){	
	uint8_t inByte = 0;         // incoming serial byte
	
	while(1) {
		if (serialCheckRxComplete()) {
			inByte = serialRead();
			
			switch (inByte)
			{
				case 'g':
				case 'G':
				//GET a byte
				//Format "gAAAAA" with AAA is address MSB first
				flashGetByteDecode();
				break;
				default:
				//Unknown command
#ifdef TALKATIVE
				serialWriteString("ERROR: unknown command: ");serialWrite(inByte);serialWriteString("\r\n");
#endif //TALKATIVE				
				break;
			}
		}
	}	//End main loop.
}

/************************************************************************/
/*  MAIN                                                                */
/************************************************************************/
int main (void)
{
	
	//Interrupts are not needed in this program. You can optionally disable interrupts.
	//asm("cli");		// DISABLE global interrupts.

		
	setupSerial();
	setupExternalFlash();
	

	establishContact();  // send a byte to establish contact until Processing responds
	

	while(1) {
		if (serialCheckRxComplete()) {
			decodeCommand();
		}
	}	//End main loop.
	return 0;
}