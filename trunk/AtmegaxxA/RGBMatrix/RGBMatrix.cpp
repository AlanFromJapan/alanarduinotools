/*
* RGBMatrix.cpp
*
* Created: 2013/01/05 18:54:14
*  Author: Alan
*/

#define F_CPU 8000000L

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include "RGBMatrix.h"
#include "SimpleAlgo.h"
#include "Font.h"
#include "DS1302.h"


#define MODE_COUNT 6

#define MODE_SEA 5
#define MODE_AUTOTIME 4
#define MODE_MIXER 3
#define MODE_NEXUS 2
#define MODE_DIGITS 1
#define MODE_SLIDE 0

volatile uint8_t mShowMode = MODE_AUTOTIME;
volatile ds1302_struct rtc;
volatile uint8_t mSubModeSwitched = 0;

/************************************************************************/
/* TIMER 2 interrupt code : calls the drawing method                    */
/************************************************************************/
ISR(TIMER2_OVF_vect){

	switch(mShowMode){
		case MODE_NEXUS:
			NexusLike();
			break;
		case MODE_DIGITS:
			if (mTiming == 400){
				mTiming = 0;
				mCount = (mCount >= 9 ? 0 : mCount+1);
	
				uint8_t vColor = rand() % 3;
				vColor = idToRGB(vColor);
	
	
				ShowOne(vColor, mCount);
	
				//matrixSlide(-1);
			}
			mTiming++;
			break;
		case MODE_SLIDE:
			SlidingTime(rtc.h24.Hour10 * 1000 + rtc.h24.Hour * 100 + rtc.Minutes10 * 10 + rtc.Minutes);
			//SlidingTime(rtc.h24.Hour10 * 1000 + rtc.h24.Hour * 100 + rtc.Seconds10 *10 + rtc.Seconds);
			break;		
		case MODE_MIXER:
			threeColors();
			break;
		case MODE_SEA:
			RandomColors();
			break;
		case MODE_AUTOTIME:
			if (rtc.Seconds10 * 10 + rtc.Seconds <= 5) {
				//the 5 first seconds of the minute (show time)
				if (mSubModeSwitched == 1){
					mSubModeSwitched = 0;
					matrixClearAll();
				}
				SlidingTime(rtc.h24.Hour10 * 1000 + rtc.h24.Hour * 100 + rtc.Minutes10 * 10 + rtc.Minutes);
			}
			else {
				if (mSubModeSwitched == 0){
					mSubModeSwitched = 1;
					matrixClearAll();
				}
				//Otherwise it's nexus
				NexusLike();
			}
			break;
	}

			
}

/************************************************************************/
/* TIMER 0 interrupt code : reads the time about every second           */
/************************************************************************/
volatile uint8_t mTimer0Divider = 0;
ISR(TIMER0_OVF_vect){
	if (mTimer0Divider == 0) {
		DS1302_clock_burst_read( (uint8_t *) &rtc);
	}	
	mTimer0Divider++;
	if (mTimer0Divider > (0xff >> 3)){
		mTimer0Divider = 0;
	}
}

/************************************************************************/
/*inits timer 2 to do interrupt on overflow (calls ISR(TIMER2_OVF_vect))*/
/************************************************************************/
void init_timer2_OVF() {
	
	//timer 2
	TCCR2 = 
		//No PWM
		(1 << FOC2) 
		//Divide by 64
		| (1 << CS21) | (1 << CS20);
	
	//trigger the interrupt vector TIMER1_OVF_vect when timer 1 is overflow
	TIMSK |= (1 << TOV2);
	
	//sets the interruptions to enabled
	sei();
}


/************************************************************************/
/*inits timer 0 to do interrupt on overflow (calls ISR(TIMER2_OVF_vect))*/
/************************************************************************/
void init_timer0_OVF() {
	
	//timer 0
	TCCR0 =
	//No PWM
	(1 << FOC0)
	//Divide by 1024
	| (1 << CS02) | (1 << CS01) | (1 << CS00);
	
	//trigger the interrupt vector TIMER1_OVF_vect when timer 1 is overflow
	TIMSK |= (1 << TOV0);
	
	//sets the interruptions to enabled
	sei();
}

int main(void)
{
	//PORTS A,C,E,D are dedicated to matrix display
	DDRD = 0xFF;
	DDRE = 0xFF;
	DDRA = 0xFF;
	DDRC = 0xFF;
	
	//G port is for input
	DDRG = 0x00;
	//pullups for everyone
	PORTG = 0xff;
	//just make sure pullups are NOT disabled
	MCUCR |= (0 << PUD);
	
	//A0-A3 : are the negative row 1-4
	//C4-C7 : are the negative row 5-8
	
	XDIV = 0x00;

	init_timer2_OVF();
	init_timer0_OVF();
	
	//DS1302 is plugged on the G port	
	setupDS1302();
			
	while (1){
		//refresh display
		showMatrix();
		
		//check if PING3 is pressed (back board button)
		if ((~PING & (1 << PING3)) != 0){
			matrixClearAll();
			mShowMode = (mShowMode + 1) % MODE_COUNT;
			_delay_ms(500);
		}
	}
	
}