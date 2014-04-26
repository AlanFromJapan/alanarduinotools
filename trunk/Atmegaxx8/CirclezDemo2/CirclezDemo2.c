/*
 * CirclezDemo2.c
 *
 * Created: 2014/03/04 23:32:53
 *  Author: Alan
 */ 

#define F_CPU 8000000L

#include <avr/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "CirclezDisplayModes.h"


#define LED_OFF { PORTD = PORTD & 0x80; }
	
#define LED1  { DDRD = 0x03;		PORTD = 0x01; }
#define LED2  { DDRD = 0x03;		PORTD = 0x02; }
#define LED3  { DDRD = 0x06;		PORTD = 0x02; }
#define LED4  { DDRD = 0x06;		PORTD = 0x04; }
#define LED5  { DDRD = 0x05;		PORTD = 0x01; }	
#define LED6  { DDRD = 0x05;		PORTD = 0x04; }	
#define LED7  { DDRD = 0x0C;		PORTD = 0x04; }
#define LED8  { DDRD = 0x0C;		PORTD = 0x08; }
#define LED9  { DDRD = 0x0A;		PORTD = 0x02; }
	
#define LED10 { DDRD = 0x0A;		PORTD = 0x08; }
#define LED11 { DDRD = 0x09;		PORTD = 0x01; }
#define LED12 { DDRD = 0x09;		PORTD = 0x08; }
#define LED13 { DDRD = 0x18;		PORTD = 0x08; }
#define LED14 { DDRD = 0x18;		PORTD = 0x10; }
#define LED15 { DDRD = 0x14;		PORTD = 0x04; }
#define LED16 { DDRD = 0x14;		PORTD = 0x10; }
#define LED17 { DDRD = 0x12;		PORTD = 0x02; }
#define LED18 { DDRD = 0x12;		PORTD = 0x10; }
#define LED19 { DDRD = 0x11;		PORTD = 0x01; }
	
#define LED20 { DDRD = 0x11;		PORTD = 0x10; }
#define LED21 { DDRD = 0x30;		PORTD = 0x10; }
#define LED22 { DDRD = 0x30;		PORTD = 0x20; }
#define LED23 { DDRD = 0x28;		PORTD = 0x08; }
#define LED24 { DDRD = 0x28;		PORTD = 0x20; }	
#define LED25 { DDRD = 0x24;		PORTD = 0x04; }
#define LED26 { DDRD = 0x24;		PORTD = 0x20; }	
#define LED27 { DDRD = 0x22;		PORTD = 0x02; }
#define LED28 { DDRD = 0x22;		PORTD = 0x20; }	
#define LED29 { DDRD = 0x21;		PORTD = 0x01; }
#define LED30 { DDRD = 0x21;		PORTD = 0x20; }	
	


#define LED31 { DDRD = 0x60;		PORTD = 0x20; }
#define LED32 { DDRD = 0x60;		PORTD = 0x40; }	
#define LED33 { DDRD = 0x50;		PORTD = 0x10; }
#define LED34 { DDRD = 0x50;		PORTD = 0x40; }	
#define LED35 { DDRD = 0x48;		PORTD = 0x08; }
#define LED36 { DDRD = 0x48;		PORTD = 0x40; }	
#define LED37 { DDRD = 0x44;		PORTD = 0x04; }
#define LED38 { DDRD = 0x44;		PORTD = 0x40; }	
#define LED39 { DDRD = 0x42;		PORTD = 0x02; }
#define LED40 { DDRD = 0x42;		PORTD = 0x40; }	
#define LED41 { DDRD = 0x41;		PORTD = 0x01; }
#define LED42 { DDRD = 0x41;		PORTD = 0x40; }	
		
	
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
		case 25: LED25; break;
		case 26: LED26; break;
		case 27: LED27; break;
		case 28: LED28; break;
		case 29: LED29; break;
		case 30: LED30; break;
		case 31: LED31; break;
		case 32: LED32; break;
		case 33: LED33; break;
		case 34: LED34; break;
		case 35: LED35; break;
		case 36: LED36; break;
		case 37: LED37; break;
		case 38: LED38; break;
		case 39: LED39; break;
		case 40: LED40; break;
		case 41: LED41; break;
		case 42: LED42; break;
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
		
	mCurrentMode = MODE_ONE_STRIP;
	
	initArrays();
	
	//setup TIMER0 : 8 byte timer
	//init_timer0_OVF();


	//button input
	//PB0 in
	DDRB = 0x00;
	//Pullup on PB0 
	PORTB = (1 << PORTB0);
	//just make sure pullups are NOT disabled
	MCUCR |= (0 << PUD);
		
	uint8_t x = 1;
	while (1){
		setLed(x);
		_delay_ms(100);
		
		x++;
		if (x > 42)
			x =1;
	}
	//LED23;
	//LED32;
		
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
