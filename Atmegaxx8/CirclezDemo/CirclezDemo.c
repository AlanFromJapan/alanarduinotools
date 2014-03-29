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
#include "CirclezDisplayModes.h"


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
#define DELAY_POV 80

#define TIMER_COUNTER 80




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


void delay_1us(uint16_t us) {
	for(uint16_t i=0;i<us;i++) _delay_us(1);
}
	

volatile uint8_t mRotateCounter = 0;

/************************************************************************/
/* TIMER 0 interrupt code : calls the drawing method                    */
/************************************************************************/
ISR(TIMER0_OVF_vect){
	for (uint8_t i = 0; i < LED_COUNT; i++){
		uint16_t vDuration = mLedVal[i] + mLedVal2[i];
		if (vDuration > 0xff)
			vDuration = 0xff;
			
		if (vDuration != 0){
			setLed(i+1);
			
			//ratio of the illumination duration 
			delay_1us((vDuration * DELAY_POV) / 256);
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
			
	//soft divider so that the data update (rotation) does not occur at each interrupt (unlike the display)
	mRotateCounter++;
	if (mRotateCounter > TIMER_COUNTER){
		mRotateCounter=0;
		
		updateArrays();
	}
}	
	
/************************************************************************/
/*inits timer 0 to do interrupt on overflow (calls ISR(TIMER2_OVF_vect))*/
/************************************************************************/
void init_timer0_OVF() {
	// Prescaler = FCPU/256
	// 8MHz / 8 bits counter / 256 divider => overflow about every 0.25ms
	TCCR0B |= (1<<CS01);
	
	//overflow of timer 0
	TIMSK0 |= (1 << TOIE0);
	
	//reset counter to zero
	TCNT0 = 0x00;
	
	//sets the interruptions to enabled
	sei();
}

	
int main(void)
{
	//factory settings is to divide internal clock 8MHz by 8.
	//don't, and just run at 8 MHz (set the clock divider to 1 so no effect)
	CLKPR = (1<<CLKPCE);
	CLKPR = 0; // Divide by 1
		
	mCurrentMode = MODE_TWO_STRIP_CHASING;
	initArrays();
	
	//setup TIMER0 : 8 byte timer
	init_timer0_OVF();


	//button input
	//PB0 in
	DDRB = 0x00;
	//Pullup on PB0 
	PORTB = (1 << PORTB0);
	//just make sure pullups are NOT disabled
	MCUCR |= (0 << PUD);
		
	//main loop
	while(1){
		//check for button press : PB0 -> change mode
		if ((~PINB & (1 << PINB0)) != 0){
			cli();
			mCurrentMode = (mCurrentMode + 1) % MODE_COUNT;
			initArrays();
			sei();
			
			//cheap debouncing
			//wait while pressed and wait again 1/2 sec (debouncing)
			while ((~PINB & (1 << PINB0)) != 0);		
			_delay_ms(500);
		}					
	}
}	
