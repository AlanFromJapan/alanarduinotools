/*
 * IronTimer.cpp
 *
 * Created: 2013/03/17 0:51:25
 *  Author: Alan
 */ 

#define F_CPU 8000000L

#include <avr/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "Rapide7seg.h"

/************************************************************************/
/* The remaining seconds in 1/10 sec                                    */
/************************************************************************/
volatile int16_t mRemainingOnDuration = 0;
#define ONE_SEC 10
#define MAX_DURATION_SEC (ONE_SEC * 60 *10)

void turnOn(){
	PORTB |= (1 << PORTB1);
}

void turnOff(){
	PORTB &= ~(1 << PORTB1);
}
	
ISR(WDT_vect){
	if (mRemainingOnDuration > ONE_SEC){
		mRemainingOnDuration -= ONE_SEC;
	}
	else {
		turnOff();
		mRemainingOnDuration = 0;	
	}
}
	
//setup the watchdog to timeout every second and make an interrupt (not a reset!)
void setupWatchdog(){
	//README : must set the fuse WDTON to 0 to enable the watchdog
	
	//disable interrupts
	cli();
	
	//make sure watchdod will be followed by a reset (must set this one to 0 because it resets the WDE bit)
	MCUSR &= ~(1 << WDRF);
	//set up WDT interrupt (from that point one have 4 cycle to modify WDTCSR)
	WDTCSR = (1<<WDCE)|(1<<WDE);
	//Start watchdog timer with 1s prescaller and interrupt only
	WDTCSR = (1<<WDIE)|(0<<WDE)|(1<<WDP2)|(1<<WDP1);
	//Enable global interrupts
	sei();
}
	
int main(void)
{
	//factory settings is to divide internal clock 8MHz by 8.
	//don't, and just run at 8 MHz (set the clock divider to 1 so no effect)
	CLKPR = (1<<CLKPCE);
	CLKPR = 0; // Divide by 1
	
	//PB1 out, PB5 & PB0 in
	DDRB = 0x02;
	//Pullup on PB0 % PB5
	PORTB = (1 << PORTB5) | (1 << PORTB0);
	//just make sure pullups are NOT disabled
	MCUCR |= (0 << PUD);
	
	setupWatchdog();
	
	setup7seg();
	
	int16_t vSpeed = 1000;
	while(1)
	{
		/*
		//1000 run makes it nearly 20 sec
		showWaitAnimation(vSpeed);
		
		vSpeed -= 20;
		if (vSpeed <= 0){
			vSpeed = 1000;
		}
		*/
		
		if (mRemainingOnDuration > ONE_SEC * 60){
			//more convert the seconds to minutes ('1+' to show remaining minutes rounded up)
			uint16_t vMinutes = 1+ (uint16_t)mRemainingOnDuration / (ONE_SEC * 60);
			//mul by 10 to shift for the display only (shows digits _XX_)
			vMinutes *= 10;
			showNumber(vMinutes, 1,2,false);
		}
		else {
			if (mRemainingOnDuration > ONE_SEC * 20){
				//until 20 sec is countdown : show seconds as is
				showNumber((uint16_t)mRemainingOnDuration, 1,2,false);
			}
			else {
				if (mRemainingOnDuration > ONE_SEC){
					//until last sec put the animation
					//1000 run makes it nearly 20 sec
					vSpeed = 1000;
					showWaitAnimation(vSpeed);
							
					vSpeed -= 20;
				}
				else {
					//zero!
					clearDisplay();
				}
			}
		}
		
		
		//check for button press : PB0 -> Add 3 minutes
		if ((~PINB & (1 << PINB0)) != 0){
			clearDisplay();
			//pushed button : turns on if not already anyway and add and extra 3 minutes
			mRemainingOnDuration +=ONE_SEC * 180;
			//max is 10 minutes
			if (mRemainingOnDuration > MAX_DURATION_SEC){
				mRemainingOnDuration = MAX_DURATION_SEC;
			}
			turnOn();
			
			//wait while pressed and wait again 1/2 sec (debouncing) 
			while ((~PINB & (1 << PINB0)) != 0);
			_delay_ms(300);
		}
		
		
		//check for button press : PB5 -> Stop now !
		if ((~PINB & (1 << PINB5)) != 0){
			clearDisplay();
			//pushed button : turns off
			mRemainingOnDuration = 0;
			turnOff();
					
			//wait while pressed and wait again 1/2 sec (debouncing)
			while ((~PINB & (1 << PINB5)) != 0);
			_delay_ms(300);
		}
	}
}