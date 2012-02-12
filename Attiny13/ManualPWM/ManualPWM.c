/*
 * ManualPWM.c
 *
 * Created: 2012/02/13 0:08:35
 *  Author: Alan
 */ 


#define F_CPU 8000000L
#include <avr/io.h>
#include <util/delay.h>

uint8_t mRGB[3];

int main(void)
{
	mRGB[0] = 150;
	mRGB[1] = 255;
	mRGB[2] = 255;

	
	//Timer full speed !
	CLKPR = (1<<CLKPCE);
	CLKPR = 0; // Divide by 1 
	
	//port B pins 0-5 go OUTPUT
    DDRB = 0xFF;
	
	uint8_t vRGBCurrentRound = 0;
    while(1)
    {
        //todo : put this in an interrupt
		if (vRGBCurrentRound == 0){
			//make pins B0-2 high
			PORTB = 
				(1 << PORTB0) 
				| (1 << PORTB1)
				| (1 << PORTB2)
				;
		}
		

		if (vRGBCurrentRound == mRGB[0] && vRGBCurrentRound != 255){
			PORTB &= ~(1 << PORTB0);
		}

	
		if (vRGBCurrentRound == mRGB[1] && vRGBCurrentRound != 255){
			PORTB &= ~(1 << PORTB1);
		}
		

		if (vRGBCurrentRound == mRGB[2] && vRGBCurrentRound != 255){
			PORTB &= ~(1 << PORTB2);
		}
		
		//and the POV
		_delay_us(50);
		vRGBCurrentRound++;
    }
}