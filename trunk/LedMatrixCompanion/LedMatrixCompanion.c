// includes
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BUFFLEN 8

unsigned char mBuffer[BUFFLEN];

void digitalWrite(unsigned char pPin, unsigned char pValue){
	if (pValue == 1) {	
		if (pPin < 8){
			PORTB = PORTB ^ (1 << pPin);
		}
		else
		if (pPin < 16){
			PORTC = PORTC ^ (1 << pPin);
		}
	}
	else {
		if (pPin < 8){
			PORTB = PORTB & ~(1 << pPin);
		}
		else
		if (pPin < 16){
			PORTC = PORTC & ~(1 << pPin);
		}	
	}
}


void setup()   {                
  for(unsigned char i =0; i < BUFFLEN; i++){
    mBuffer[i] = 0;
  }  
/*

  for(int i =0; i < 16; i++){
    digitalWrite(i, 1);  
  }  
*/
}

void randomOnOff(){
  int l,c;
  l = random(0,8);
  c = random(0,8);  
  unsigned char b = mBuffer[c];
  b = b ^ _BV(l);
  mBuffer[c] = b;

}

unsigned char mPos = 0;
void scanLines(){
  unsigned char l,c;
  l = mPos / 8;
  c = mPos % 8;  
  unsigned char b = mBuffer[c];
  b = b ^ _BV(l);
  mBuffer[c] = b;

  mPos = (mPos + 1) % 64;
}

void loop()                     
{
	//change the drawing
	//randomOnOff();
	scanLines();

	//draw the in memory image
	for (unsigned char c = 0; c < BUFFLEN; c++){
		PORTD = mBuffer[c];
		PORTB = ~_BV(c);
		
		//PORTC = (c >= 6 ? ~_BV(c - 6) : 0xFF);
		//PORTC = ((mBuffer[c] & 0x03) << 2);

		//x is the 2 first bit of the PORTD data. Don't use port D to free the pins 0&1 TXand RX of the serial communication.
		//y is the last part of the pins to display. Pins A0 & A1 (D14 & D15)
		//x is 0b------XX, y is 0b------YY we want to make 0b0000XXYY
		unsigned char x = (mBuffer[c] & 0x03);
		unsigned char y = (c >= 6 ? ~_BV(c - 6) : 0xFF);
		PORTC = 0x0F & ( (x << 2) | (y & 0x03)); 
		
		//stay displayed to allow POV
		_delay_us(1000);
		PORTD = 0;
		PORTB = 0xFF;
		PORTC = 0xFF;
	}

}


//
// main function
//
int main(void) {
	//all ports go output 
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF;
	
	for (;;) loop();
}



