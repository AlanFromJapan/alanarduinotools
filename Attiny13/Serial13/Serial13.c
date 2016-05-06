/*
 * Serial13.c
 * Bitbanging of serial protocol (SENDING only - 9600/8N1)
 *
 * Created: 2016/05/06 16:39:28
 *  Author: Alan
 */ 


#define F_CPU			4800000

#include <avr/io.h>
#include <avr/delay.h>


//One bit is 104 uS -> You have to tweak this value to match your crystal OR get a precise crystal. 
//In my case, I use the infamous internal crystal of my ATtiny 13 which is 9.6% off !!
#define SER_BIT_US		114

#define SER_PORT		PORTB
#define SET_TX_PIN		PB3

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

/************************************************************************/
/* MAIN                                                                 */
/************************************************************************/
int main(void)
{
	//DO NOT FORGET this, otherwise your output will be ugly (not square signal)
	//Set pin mode as OUTPUT
	DDRB = DDRB | (1 << SET_TX_PIN);
	
    while(1)
    {
		for (uint8_t lettre = 'A'; lettre <= 'Z'; lettre++){
		   serialSendChar(lettre);
		
		}		
			_delay_ms(3000);
    }
}