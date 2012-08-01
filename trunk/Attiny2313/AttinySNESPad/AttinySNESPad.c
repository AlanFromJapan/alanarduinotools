/*
 * AttinySNESPad.c
 * Howto make your SNES pad compatible controller with a attiny2313.
 * http://kalshagar.wikispaces.com/Around+a+SNES
 * 
 * Created: 2012/04/29 12:08:04
 *  Author: AlanFromJapan
 * Sources:
 *  http://www.gamesx.com/controldata/snesdat.htm
 *  http://www.gamesx.com/controldata/nessnes.htm
 *
 * Interrupts:
 *  http://www.windmeadow.com/node/19
 *  http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
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

volatile uint8_t mCurrentButtonIndex = 0;

volatile uint16_t mButtonStatus = 0xFFFF; // all UP (nothing pressed)


void ReadButtonsStatus_TESTReadSetDown2() 
{
	mButtonStatus = 0xFFFF; // all UP (nothing pressed)
	
	if ((PINB & (1 << 1)) == 0) {
		//button pressed
		mButtonStatus ^= (1 << ORDER_BUTTON_START);
	}
	
	if ((PINB & (1 << 2)) == 0) {
		//button pressed. DONT FORGET to force the stupid compiler to work with uint16!!
		mButtonStatus ^= (uint16_t)(1 << ORDER_BUTTON_RIGHT);
	}

}

//Write one value on the DATA line
//inline : in short the compiler will copy the code and not make a jump to function, to go faster.
//no pointer, short, need of extra speed : the perfect client for /inline/.
inline void WriteNextData(){
	//push the first value on the clock (will be read on CLOCK going DOWN)
	if ((mButtonStatus & (1 << mCurrentButtonIndex)) == 0){
		PORTA = 0x00;
	}
	else {
		PORTA = 0xFF;
	}
	
	//go next
	mCurrentButtonIndex++;
}

//Handles the INT1 pin : LATCH signal going DOWN (falling hedge) : STARTUP 
SIGNAL (SIG_INT1)
{ 
	//read button status before the clock ticks in
	ReadButtonsStatus_TESTReadSetDown2();
	mCurrentButtonIndex = 0;
	
	//put already the first bit on the rail for reading 
	WriteNextData();
}

//Handles the INT0 pin : CLOCK signal going DOWN (falling hedge) = reading
// push next value to the value pin
SIGNAL (SIG_INT0)
{ 
	//move next
	WriteNextData();
}

void setupInterrupt(){
	//use INT1 and INT0
	PCMSK |= (1 << PIND3) | (1 << PIND2);
	
	// interrupt on INT1 pin falling edge 
	//INT0 goes on falling hedge
	MCUCR |= (1<<ISC11) | (1<<ISC01); // | (1<<ISC00);

	// turn on interrupts!
	GIMSK  |= (1<<INT1) | (1<<INT0);
	
	//without a call to sei() it doesn't work 
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
	//PORTA input + DDRA = 1 => pullups
	//PORTA = 0xFF;
	
	//port d as input on all pins (buttons and INT0 and INT1)
	DDRD = 0x00;
	
	//port b is only inputs (the buttons), with pullups
	DDRB = 0x00;
	PORTB = 0xFF; //pullups
	MCUCR |= (0 << PUD); //just make sure pullups are NOT disabled
	
	//turn on the interrupts
	setupInterrupt();
	
	
    while(1)
    {
		//all the job is done inn the interrupt, just loop 'till the end of time
    }
}