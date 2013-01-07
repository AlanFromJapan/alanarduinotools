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


/*
register volatile uint8_t mCurrentButtonIndex asm("r2");

register volatile uint8_t mButtonStatusL asm("r3");
register volatile uint8_t mButtonStatusH asm("r4");
*/

volatile uint8_t mCurrentButtonIndex = 0;

volatile uint16_t mButtonStatus = 0xFFFF; // all UP (nothing pressed)


void ReadButtonsStatus() 
{
	mButtonStatus = 0xFFFF; // all UP (nothing pressed)
	
	if ((PINB & (1 << 0)) == 0) {
		//button pressed
		mButtonStatus ^= (uint16_t)(1 << ORDER_BUTTON_START);
	}
	
	if ((PINB & (1 << 1)) == 0) {
		//button pressed
		mButtonStatus ^= (uint16_t)(1 << ORDER_BUTTON_SELECT);
	}
		
	if ((PINB & (1 << 2)) == 0) {
		//button pressed
		mButtonStatus ^= (uint16_t)(1 << ORDER_BUTTON_X);
	}
		
	if ((PINB & (1 << 3)) == 0) {
		//button pressed
		mButtonStatus ^= (uint16_t)(1 << ORDER_BUTTON_Y);
	}
		
	if ((PINB & (1 << 4)) == 0) {
		//button pressed
		mButtonStatus ^= (uint16_t)(1 << ORDER_BUTTON_A);
	}
		
	if ((PINB & (1 << 5)) == 0) {
		//button pressed
		mButtonStatus ^= (uint16_t)(1 << ORDER_BUTTON_B);
	}
		
	if ((PINB & (1 << 6)) == 0) {
		//button pressed
		mButtonStatus ^= (uint16_t)(1 << ORDER_BUTTON_R);
	}
		
	if ((PINB & (1 << 7)) == 0) {
		//button pressed
		mButtonStatus ^= (uint16_t)(1 << ORDER_BUTTON_L);
	}
	
	////////////////////////////////////////////////////////////////////////////
	// The joystick wires are close together, use D0-D1-A1-A0
	
	if ((PIND & (1 << 0)) == 0) {
		//button pressed. DONT FORGET to force the stupid compiler to work with uint16!!
		mButtonStatus ^= (uint16_t)(1 << ORDER_BUTTON_UP);
	}

	
	if ((PIND & (1 << 1)) == 0) {
		//button pressed. DONT FORGET to force the stupid compiler to work with uint16!!
		mButtonStatus ^= (uint16_t)(1 << ORDER_BUTTON_DOWN);
	}
	
		
	if ((PINA & (1 << 1)) == 0) {
		//button pressed. DONT FORGET to force the stupid compiler to work with uint16!!
		mButtonStatus ^= (uint16_t)(1 << ORDER_BUTTON_RIGHT);
	}
	
		
	if ((PINA & (1 << 0)) == 0) {
		//button pressed. DONT FORGET to force the stupid compiler to work with uint16!!
		mButtonStatus ^= (uint16_t)(1 << ORDER_BUTTON_LEFT);
	}
}



//Write one value on the DATA line
//inline : in short the compiler will copy the code and not make a jump to function, to go faster.
//no pointer, short, need of extra speed : the perfect client for /inline/.
//inline 
void WriteNextData(){
	//push the first value on the clock (will be read on next CLOCK going DOWN)
	if ((mButtonStatus & (uint16_t)(1 << mCurrentButtonIndex)) == 0){
		PORTD &= ~(1 << 4); //make D4 = 0
	}
	else {
		PORTD |= (1 << 4);  //make D4 = 1
	} 
	
	//go next
	mCurrentButtonIndex++;
}

//Handles the INT1 pin : LATCH signal going DOWN (falling hedge) : STARTUP 
SIGNAL (INT1_vect)
{ 
	mCurrentButtonIndex = 0;
	
	//put already the first bit on the rail for reading 
	WriteNextData();
}


//Handles the INT0 pin : CLOCK signal going UP (rising hedge) = put data that will be read on falling hedge in 6uS
// push next value to the value pin
SIGNAL (INT0_vect)
{ 
	//move next
	WriteNextData();		
	
	//go next
	mCurrentButtonIndex++;
	
	if (mCurrentButtonIndex >= 12){
		//read button status for next round (too long to read BEFORE the first tick)
		ReadButtonsStatus();
	}
}



void setupInterrupt(){
	//use INT1 and INT0
	PCMSK |= (1 << PIND3) | (1 << PIND2);
	
	// interrupt on INT1 pin falling edge (prepare for reading)
	//INT0 goes on rising hedge (send value on raise, will be read on fall)
	MCUCR |= (1<<ISC11) | (1<<ISC01) | (1<<ISC11); 

	// turn on interrupts!
	GIMSK  |= (1<<INT1) | (1<<INT0);
	
	//without a call to sei() it doesn't work 
	sei();
}

int main(void)
{
	CannotCompile
	This version has a wrong timing, so don't use it. Use the AsmSNESPAD that works fine.
	This is just kept for memory.
	/CannotCompile
	
	//factory settings is to divide internal clock 8MHz by 8.
	//don't, and just run at 8 MHz (set the clock divider to 1 so no effect)
	CLKPR = (1<<CLKPCE);
	CLKPR = 0; // Divide by 1 
	
	//port A is all input with pullups
	DDRA = 0x00;
	PORTA = 0xFF;
	
	//port d as input on all pins (buttons and INT0 and INT1) EXCEPT D4 that is DATA (output)
	DDRD = 0x10;
	//pullups everywhere except D2,D3,D4 (INT0, INT1 and DATA)
	PORTD = 0b11100011;
	
	//port b is only inputs (the buttons), with pullups
	DDRB = 0x00;
	PORTB = 0xFF; //pullups
	
	//just make sure pullups are NOT disabled
	MCUCR |= (0 << PUD); 
	
	//by default buttons are all up
	mButtonStatus = 0xffff;
	
	//turn on the interrupts
	setupInterrupt();
	
	
    while(1)
    {
		//all the job is done inn the interrupt, just loop 'till the end of time
    }
}