/*
 * IronTimer.cpp
 *
 * Created: 2013/03/17 0:51:25
 *  Author: Alan
 */ 


#include <avr/io.h>
#include "Rapide7seg.h"

float mLatestPower = 1234;

int main(void)
{
	//factory settings is to divide internal clock 8MHz by 8.
	//don't, and just run at 8 MHz (set the clock divider to 1 so no effect)
	CLKPR = (1<<CLKPCE);
	CLKPR = 0; // Divide by 1
	
	DDRB = 0x02;
	PORTB = 0x00;
	
	setup7seg();
	int16_t vSpeed = 1000;
	while(1)
	{
		showWaitAnimation(vSpeed);
		
		vSpeed -= 20;
		if (vSpeed <= 0){
			vSpeed = 1000;
		}
		
		//PORTB = ~PORTB;
	}
}