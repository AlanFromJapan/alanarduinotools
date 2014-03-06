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

#define LED_COUNT 24

#define LED_OFF { PORTC = PORTC & 0xC0; PORTD = PORTD & 0xC0; }
	
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
#define DELAY_POV 110

#define DELAY_SLEEP_MIN 10
#define DELAY_SLEEP_MAX 100
#define DELAY_SLEEP_DELTA 10

uint8_t mLedVal[LED_COUNT];

void setLed(uint8_t pLed){
	switch(pLed){
		case 1: LED1; break;
		case 2: LED2; break;
		case 3: LED3; break;
		case 4: LED4; break;
		case 5: LED5; break;
		case 6: LED6; break;
		case 7: LED7; break;
		case 8: LED8; break;
		case 9: LED9; break;
		case 10: LED10; break;
		case 11: LED11; break;
		case 12: LED12; break;
		case 13: LED13; break;
		case 14: LED14; break;
		case 15: LED15; break;
		case 16: LED16; break;
		case 17: LED17; break;
		case 18: LED18; break;
		case 19: LED19; break;
		case 20: LED20; break;
		case 21: LED21; break;
		case 22: LED22; break;
		case 23: LED23; break;
		case 24: LED24; break;
	}
}

void endlessCircle() {
	 while(1)
	 {
		 setLed(1);_delay_ms(DELAY_SLEEP);
		 setLed(2);_delay_ms(DELAY_SLEEP);
		 setLed(3);_delay_ms(DELAY_SLEEP);
		 setLed(4);_delay_ms(DELAY_SLEEP);
		 setLed(5);_delay_ms(DELAY_SLEEP);
		 setLed(6);_delay_ms(DELAY_SLEEP);
		 setLed(7);_delay_ms(DELAY_SLEEP);
		 setLed(8);_delay_ms(DELAY_SLEEP);
		 setLed(9);_delay_ms(DELAY_SLEEP);
		 setLed(10);_delay_ms(DELAY_SLEEP);
		 setLed(11);_delay_ms(DELAY_SLEEP);
		 setLed(12);_delay_ms(DELAY_SLEEP);
		 setLed(13);_delay_ms(DELAY_SLEEP);
		 setLed(14);_delay_ms(DELAY_SLEEP);
		 setLed(15);_delay_ms(DELAY_SLEEP);
		 setLed(16);_delay_ms(DELAY_SLEEP);
		 setLed(17);_delay_ms(DELAY_SLEEP);
		 setLed(18);_delay_ms(DELAY_SLEEP);
		 setLed(19);_delay_ms(DELAY_SLEEP);
		 setLed(20);_delay_ms(DELAY_SLEEP);
		 setLed(21);_delay_ms(DELAY_SLEEP);
		 setLed(22);_delay_ms(DELAY_SLEEP);
		 setLed(23);_delay_ms(DELAY_SLEEP);
		 setLed(24);_delay_ms(DELAY_SLEEP);
		 
	 }
}

void delay_1us(uint16_t us) {
	for(uint16_t i=0;i<us;i++) _delay_us(1);
}
	
void rotateArray(int8_t pDirection){
	if (pDirection > 0){
		uint8_t vTemp = mLedVal[LED_COUNT-1];
		uint8_t i = LED_COUNT-2; 
		do 
		{
			mLedVal[i+1] = mLedVal[i];
			i--;
		} while (i > 0);
		mLedVal[1] = mLedVal[0];
		mLedVal[0] = vTemp;
	}
	else{
		uint8_t vTemp = mLedVal[0];
		uint8_t i = 1;
		do
		{
			mLedVal[i-1] = mLedVal[i];
			i++;
		} while (i < LED_COUNT);
		mLedVal[LED_COUNT-1] = vTemp;
	}
}
	
int main(void)
{
	for (uint8_t i = 0; i < LED_COUNT; i++){
		mLedVal[1] = 0;	
	}
	
	mLedVal[0] = 255;
	mLedVal[1] = 192;
	mLedVal[2] = 128;
	mLedVal[3] = 64;
	mLedVal[4] = 20;
	mLedVal[5] = 10;
	mLedVal[6] = 5;

	uint16_t vCounter = 0;
		
	while(1){
		for (uint8_t i = 0; i < LED_COUNT; i++){
			if (mLedVal[i] != 0){
				setLed(i+1);
				delay_1us((mLedVal[i] * DELAY_POV) / 256);
			}
			//with this code the display becomes roughly time constant, meaning
			//that disregarding the number of lit led they will have the same quantum of lighting time.
			//by disabling that the leds are brighter and you wont see the difference if less than 10 leds
			//are on.
			//else{
				//delay_1us(DELAY_POV);
			//}
			LED_OFF;
		}		
		
		vCounter++;
		if (vCounter >80){
			vCounter=0;
			rotateArray(-1);
		}
		
	}
}	
