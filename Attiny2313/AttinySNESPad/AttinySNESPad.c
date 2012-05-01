/*
 * AttinySNESPad.c
 * Howto make your SNES pad compatible controller with a attiny2313.
 * 
 * Created: 2012/04/29 12:08:04
 *  Author: Alan
 * Sources:
 *  http://www.gamesx.com/controldata/snesdat.htm
 *  http://www.gamesx.com/controldata/nessnes.htm
 *
 * Interrupts:
 *  http://www.windmeadow.com/node/19
 * 
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

/*
1 	B
2 	Y
3 	Select
4 	Start
5 	Up on joypad
6 	Down on joypad
7 	Left on joypad
8 	Right on joypad
9 	A
10 	X
11 	L
12 	R
13-16 	none (always high)
*/
#define ORDER_BUTTON_B			0
#define ORDER_BUTTON_Y			1
#define ORDER_BUTTON_SELECT		2
#define ORDER_BUTTON_START		3
#define ORDER_BUTTON_UP			4
#define ORDER_BUTTON_DOWN		5
#define ORDER_BUTTON_LEFT		6
#define ORDER_BUTTON_RIGHT		7
#define ORDER_BUTTON_A			8
#define ORDER_BUTTON_X			9
#define ORDER_BUTTON_L			10
#define ORDER_BUTTON_R			11
#define TOTALORDER				16


#define PIN_CLOCK				0
#define PIN_DATA				1

volatile uint16_t mCountTest = 0;

//Handles the INT1 pin : LATCH signal going DOWN (falling hedge)
SIGNAL (SIG_INT1)
{ 
    // Do some stuff
/*
	//blink
	PORTD ^= (1 << 0);
	_delay_ms(1000);
	PORTD ^= (1 << 0);
*/

	//in 6us will start the reception of the first clock signal
	//clock is normal low, goes high for 6us then sleep again 6us : the SNES will sample the DATA line on the FALLING hedge
	for (int vRound = 0; vRound < TOTALORDER; vRound++ ){
		
		//wait for CLOCK pin to go HIGH and set the value
		while ((PORTA & (1 <<PIN_CLOCK)) == 0){} 
		
		//HIGH means NOT PRESSED
		PORTA = 0xFF;
		
		/*
		//for the test, just send UP
		if (mCountTest % 120 == 0 && vRound == ORDER_BUTTON_UP){
			PORTA = (0 << PIN_DATA);
		}
		else {
			PORTA = (1 << PIN_DATA);
		}
		*/
		
		//for the test, just send right
		if (vRound == ORDER_BUTTON_START){
			PORTA = 0x00;// (0 << PIN_DATA);
		}
		else {
			PORTA = 0xFF; // (1 << PIN_DATA);
		}
		
		
		//wait for CLOCK pin to go LOW and let's roll for next turn...
		while ((PORTA & (1 <<PIN_CLOCK)) != 0){} 
	}
	
	mCountTest++;
}

void setupInterrupt(){
	//use INT1
	PCMSK |= (1 << PIND3);
	
	// interrupt on INT1 pin falling edge (sensor triggered) 
	MCUCR |= (1<<ISC11);

	// turn on interrupts!
	GIMSK  |= (1<<INT1);
	
	//without a call to sei() it doesn't work it seems...
	sei();
}

int main(void)
{
	//factory settings is to divide internal clock 8MHz by 8.
	//don't, and just run at 8 MHz (set the clock divider to 1 so no effect)
	CLKPR = (1<<CLKPCE);
	CLKPR = 0; // Divide by 1 
	
	
	//port a : CLOCK in, DATA out
	DDRA = (0 << PIN_CLOCK) | (1 << PIN_DATA);
	//port d as output except the int1 (PD3)
	DDRD = 0xFF ^ (1 << PD3);
	
	//turn on the interrupts
	setupInterrupt();
	
	
    while(1)
    {
		/*
		//loop
		PORTD ^= (1 << 1);
		_delay_ms(300);
		PORTD ^= (1 << 1);
		_delay_ms(1200);
		*/
    }
}