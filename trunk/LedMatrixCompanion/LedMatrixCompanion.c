// includes
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//Excellent sample of circular buffer under ATmega 88
//http://jimsmindtank.com/how-to-atmega-usart-in-uart-mode-with-circular-buffers/
////////////////////////////////
//		Buffer Stuff
#include <avr/interrupt.h>

#define MATRIXBUFFLEN 8

unsigned char mBuffer[MATRIXBUFFLEN];

#define rx_buffer_size 64
#define rx_buffer_mask (rx_buffer_size - 1)

volatile unsigned char rx_buffer_tail = 0;
volatile unsigned char rx_buffer_head = 0;
volatile unsigned char rx_buffer[rx_buffer_size];
volatile unsigned char rx_buffer_overflow_flag;

unsigned char data_in_buffer(void);
unsigned char read_from_buffer(void);
void init_usart_rx_buffer(void);
////////////////////////////////

void init_usart(unsigned long);


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


void resetMatrixBuffer()   {                
  for(unsigned char i =0; i < MATRIXBUFFLEN; i++){
    mBuffer[i] = 0;
  }  
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
	for (unsigned char c = 0; c < MATRIXBUFFLEN; c++){
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
	//for the matrix
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF;
	
	//init the serial port
	init_usart(9600);
	DDRB = 0xff;
	init_usart_rx_buffer();
	unsigned char vCharacter = 0;

	for(;;)
	{
		if(data_in_buffer() != 0)
		{
			vCharacter = read_from_buffer();
			//echo
			while((UCSR0A & (1<<UDRE0)) == 0) {}
			UDR0 = vCharacter;
			
			if (vCharacter == 'R'){
				resetMatrixBuffer();
			}
		}
		_delay_ms(10);
		
		//loop the matrix drawing
		loop();
	}

}


void init_usart (unsigned long baud)
{

	unsigned int UBRR_2x_off;
	unsigned int UBRR_2x_on;
	unsigned long closest_match_2x_off;
	unsigned long closest_match_2x_on;
	unsigned char off_2x_error;
	unsigned char on_2x_error;

	UBRR_2x_off = F_CPU/(16*baud) - 1;
	UBRR_2x_on = F_CPU/(8*baud) - 1;

	closest_match_2x_off = F_CPU/(16*(UBRR_2x_off + 1));
	closest_match_2x_on = F_CPU/(8*(UBRR_2x_on + 1));

	off_2x_error = 255*(closest_match_2x_off/baud - 1);
	if (off_2x_error <0) {off_2x_error *= (-1);}
	on_2x_error = 255*(closest_match_2x_on/baud -1);
	if (on_2x_error <0) {on_2x_error *= (-1);}

	if(baud > F_CPU / 16)
	{
		UBRR0L = 0xff & UBRR_2x_on;
		UBRR0H = 0xff & (UBRR_2x_on>>8);
		UCSR0A |= (1<<U2X0);
	} else {

		if (off_2x_error > on_2x_error)
		{
			UBRR0L = 0xff & UBRR_2x_on;
			UBRR0H = 0xff & (UBRR_2x_on>>8);
			UCSR0A |= (1<<U2X0);
		} else {
			UBRR0L = 0xff & UBRR_2x_off;
			UBRR0H = 0xff & (UBRR_2x_off>>8);
			UCSR0A &= ~(1<<U2X0);
		}
	}

	UCSR0B = (0<<RXCIE0) |
	(0<<TXCIE0) |
	(0<<UDRIE0) |
	(1<<RXEN0) |
	(1<<TXEN0) |
	(0<<UCSZ02);

	UCSR0A = (0<<U2X0) |
	(0<<MPCM0) ;

	UCSR0C = (0<<UMSEL01) | (0<<UMSEL00) |
	(0<<UPM01) | (0<<UPM00) |
	(0<<USBS0) |
	(1<<UCSZ01) | (1<<UCSZ00) |
	(0<<UCPOL0) ;
}

//////////////////////////////
//		Buffer Stuff
void init_usart_rx_buffer(void)
{
	UCSR0B |= (1<<RXCIE0);
	sei();
}

unsigned char data_in_buffer(void)
{
	if(rx_buffer_head == rx_buffer_tail)
	{
		return 0;
	} else {
		return 1;
	}
}

unsigned char read_from_buffer(void)
{
	while(rx_buffer_head == rx_buffer_tail) {}
	rx_buffer_tail = (rx_buffer_tail + 1) & rx_buffer_mask;
	return rx_buffer[rx_buffer_tail];
}

ISR(USART_RX_vect)
{
	if(((rx_buffer_head+1)&rx_buffer_mask) == rx_buffer_tail)
	{
		rx_buffer_overflow_flag = UDR0;
		rx_buffer_overflow_flag = 1;
	}	else	{
		rx_buffer_head = (rx_buffer_head+1)&rx_buffer_mask;
		rx_buffer[rx_buffer_head] = UDR0;
	}
}
//////////////////////////////

