/*
 * CirclezDemo.c
 *
 * Created: 2014/03/04 23:32:53
 *  Author: Alan
 */ 

#define F_CPU 8000000L

#include <avr/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#define LED_OFF { PORTC = PORTC & 0xC0; }
	
#define LED1  { DDRC = 0x03;		PORTC = 0b00000001; DDRD = 0x00; }
#define LED2  { DDRC = 0x05;		PORTC = 0b00000001; DDRD = 0x00; }
#define LED3  { DDRC = 0x06;		PORTC = 0b00000100; DDRD = 0x00; }
#define LED4  { DDRC = 0x05;		PORTC = 0b00000100; DDRD = 0x00; }
#define LED5  { DDRC = 0x03;		PORTC = 0b00000010; DDRD = 0x00; }
#define LED6  { DDRC = 0x06;		PORTC = 0b00000010; DDRD = 0x00; }
	
#define LED7  { DDRC = 0b00011000; PORTC = 0b00001000; DDRD = 0x00; }
#define LED8  { DDRC = 0b00101000; PORTC = 0b00001000; DDRD = 0x00; }
#define LED9  { DDRC = 0b00110000; PORTC = 0b00100000; DDRD = 0x00; }
#define LED10 { DDRC = 0b00101000; PORTC = 0b00100000; DDRD = 0x00; }
#define LED11 { DDRC = 0b00011000; PORTC = 0b00010000; DDRD = 0x00; }
#define LED12 { DDRC = 0b00110000; PORTC = 0b00010000; DDRD = 0x00; }

#define LED13 { DDRD = 0x03;		PORTD = 0b00000001; DDRC = 0x00; }
#define LED14 { DDRD = 0x05;		PORTD = 0b00000001; DDRC = 0x00; }
#define LED15 { DDRD = 0x06;		PORTD = 0b00000100; DDRC = 0x00; }
#define LED16 { DDRD = 0x05;		PORTD = 0b00000100; DDRC = 0x00; }
#define LED17 { DDRD = 0x03;		PORTD = 0b00000010; DDRC = 0x00; }
#define LED18 { DDRD = 0x06;		PORTD = 0b00000010; DDRC = 0x00; }

#define LED19 { DDRD = 0b00011000;	PORTD = 0b00001000; DDRC = 0x00; }
#define LED20 { DDRD = 0b00101000;	PORTD = 0b00001000; DDRC = 0x00; }
#define LED21 { DDRD = 0b00110000;	PORTD = 0b00100000; DDRC = 0x00; }
#define LED22 { DDRD = 0b00101000;	PORTD = 0b00100000; DDRC = 0x00; }
#define LED23 { DDRD = 0b00011000;	PORTD = 0b00010000; DDRC = 0x00; }
#define LED24 { DDRD = 0b00110000;	PORTD = 0b00010000; DDRC = 0x00; }	
	
#define DELAY_SLEEP 25

#define DELAY_SLEEP_MIN 10
#define DELAY_SLEEP_MAX 100
#define DELAY_SLEEP_DELTA 10


int main(void)
{
	uint8_t vDelay = DELAY_SLEEP_MIN;
	int8_t vDelayDelta = DELAY_SLEEP_DELTA;
	
    while(1)
    {
		//PORTC = vC; 
		//
		////vC++;
		//vC = vC << 1;
		//if (vC == 0){
			//vC = 0x01;
		//}
		
		LED1 ;_delay_ms(DELAY_SLEEP);
		LED2 ;_delay_ms(DELAY_SLEEP);
		LED3 ;_delay_ms(DELAY_SLEEP);
		LED4 ;_delay_ms(DELAY_SLEEP);
		LED5 ;_delay_ms(DELAY_SLEEP);
		LED6 ;_delay_ms(DELAY_SLEEP);
		
		LED7 ;_delay_ms(DELAY_SLEEP);
		LED8 ;_delay_ms(DELAY_SLEEP);
		LED9 ;_delay_ms(DELAY_SLEEP);
		LED10;_delay_ms(DELAY_SLEEP);
		LED11;_delay_ms(DELAY_SLEEP);
		LED12;_delay_ms(DELAY_SLEEP);
		
		LED13;_delay_ms(DELAY_SLEEP);
		LED14;_delay_ms(DELAY_SLEEP);
		LED15;_delay_ms(DELAY_SLEEP);
		LED16;_delay_ms(DELAY_SLEEP);
		LED17;_delay_ms(DELAY_SLEEP);
		LED18;_delay_ms(DELAY_SLEEP);
		
		LED19;_delay_ms(DELAY_SLEEP);
		LED20;_delay_ms(DELAY_SLEEP);
		LED21;_delay_ms(DELAY_SLEEP);
		LED22;_delay_ms(DELAY_SLEEP);
		LED23;_delay_ms(DELAY_SLEEP);
		LED24;_delay_ms(DELAY_SLEEP);
		
		//vDelay = vDelay + vDelayDelta;
		//if (vDelay <= DELAY_SLEEP_MIN || vDelay >= DELAY_SLEEP_MAX)
			//vDelayDelta = -vDelayDelta;
    }
}