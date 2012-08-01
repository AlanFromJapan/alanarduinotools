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

volatile uint16_t mCountTest = 0;

volatile uint16_t mButtonStatus = 0xFFFF; // all UP (nothing pressed)

volatile uint8_t mTestFlag = 0;

void SendButtonStatusTEST2() 
{
	PORTA = 0xFF;

	if (mCountTest == 600){
		mButtonStatus = (1 << ORDER_BUTTON_START);
		
		for (uint8_t i = 0; i < 16; i++){
			if ((mButtonStatus & (1 << i)) == 0){
				PORTA = 0x00;
			}
			else {
				PORTA = 0xFF;
			}
			_delay_loop_1(12); 
		}
	}	
	
	if (mCountTest > 600 && mCountTest % 120 == 0){
		mTestFlag++;
		//if (mTestFlag % 2 == 0){
			//mButtonStatus = (1 << ORDER_BUTTON_RIGHT);
		//}	
		//if (mTestFlag % 2 == 1){
			//mButtonStatus = (1 << ORDER_BUTTON_DOWN);
		//}
		
		//mButtonStatus = ~mButtonStatus;
		
		//mButtonStatus = ~(1 << (mTestFlag % 16));
		
		_delay_loop_1(1); 
		if (mTestFlag % 2 == 0){
			mButtonStatus = ~(1 << ORDER_BUTTON_LEFT);
		}
		else {
			mButtonStatus = ~(1 << ORDER_BUTTON_RIGHT);
		}
		
		for (uint8_t i = 0; i < 16; i++){
			if ((mButtonStatus & (1 << i)) == 0){
				PORTA = 0x00;
			}
			else {
				PORTA = 0xFF;
			}
			_delay_loop_1(12); 
		}
	}	
		
	PORTA = 0xFF;	

	mCountTest++;
}

void SendButtonStatusTEST() 
{
	PORTA = 0xFF;

	if (mCountTest % 180 == 0){
		mButtonStatus = (1 << ORDER_BUTTON_START);
		
		for (uint8_t i = 0; i < 16; i++){
			if ((mButtonStatus & (1 << i)) == 0){
				PORTA = 0x00;
			}
			else {
				PORTA = 0xFF;
			}
			_delay_loop_1(12); 
		}
	}	
		
	PORTA = 0xFF;	

	mCountTest++;
}

void SendButtonStatusTESTAAAA() 
{
	PORTA = 0xFF;

	
		mButtonStatus = 0xAAAA;
		
		for (uint8_t i = 0; i < 16; i++){
			if ((mButtonStatus & (1 << i)) == 0){
				PORTA = 0x00;
			}
			else {
				PORTA = 0xFF;
			}
			_delay_loop_1(12); 
		}
	
		
	PORTA = 0xFF;	

	mCountTest++;
}

void SendButtonStatus() 
{
	PORTA = 0xFF;

	for (uint8_t i = 0; i < 16; i++){
		if ((mButtonStatus & (1 << i)) == 0){
			PORTA = 0x00;
		}
		else {
			PORTA = 0xFF;
		}
		_delay_loop_1(12); 
	}
		
	PORTA = 0xFF;	

	mCountTest++;
}

//Read the buttons status and store it into the global variable mButtonStatus
void ReadButtonsStatus() 
{
	mButtonStatus = 0xFFFF; // all UP (nothing pressed)
	
	if ((PORTB & (1 << 0)) == 0) {
		//button pressed
		mButtonStatus ^= (1 << ORDER_BUTTON_START);
	}
	
	mButtonStatus = ~mButtonStatus;
}


void ReadButtonsStatus_TESTReadSetDown() 
{
	mButtonStatus = 0xFFFF; // all UP (nothing pressed)
	
	if ((PINB & (1 << 0)) == 0) {
		//button pressed
		mButtonStatus = 0x0000;
	}
}

void ReadButtonsStatus_TESTReadSetDown2() 
{
	mButtonStatus = 0xFFFF; // all UP (nothing pressed)
	
	if ((PINB & (1 << 1)) == 0) {
		//button pressed
		mButtonStatus = 0xFFFF ^ (1 << ORDER_BUTTON_START);
	}
}

//Fake, return starts
void ReadButtonsStatus_TestSTART() 
{
	mButtonStatus = 0x0000; // all UP (nothing pressed)
	
		mButtonStatus ^= (1 << ORDER_BUTTON_START);
	
	mButtonStatus = ~mButtonStatus;
}

//fake return 0x0000
void ReadButtonsStatus_TestAllDown() 
{
	mButtonStatus = 0x0000; // all down
}

//fake return 0b1010101010
void ReadButtonsStatus_TestAlternate() 
{
	mButtonStatus = 0b1010101010101010; // all down
}

//Fake, return starts once in a while
void ReadButtonsStatus_TestSTART2() 
{
	if (mTestFlag == 0 ){
	
	
		//all down
		mButtonStatus = 0x0000;
	
	}
	else {
		//all up
		mButtonStatus = 0xFFFF;
	}	
	
	mTestFlag++;
}



//Handles the INT1 pin : LATCH signal going DOWN (falling hedge) : STARTUP
SIGNAL (SIG_INT1)
{ 
	//read button status before the clock ticks in
	ReadButtonsStatus_TESTReadSetDown2();
	mCountTest = 0;
	
	if ((mButtonStatus & (1 << mCountTest)) == 0){
		PORTA = 0x00;
	}
	else {
		PORTA = 0xFF;
	}
}

//Handles the INT0 pin : LATCH signal going DOWN (falling hedge) : CLOCK
SIGNAL (SIG_INT0)
{ 
	
	if ((mButtonStatus & (1 << mCountTest)) == 0){
		PORTA = 0x00;
	}
	else {
		PORTA = 0xFF;
	}
		
	mCountTest++;
}

void setupInterrupt(){
	//use INT1 and INT0
	PCMSK |= (1 << PIND3) | (1 << PIND2);
	
	// interrupt on INT0/INT1 pin falling edge (sensor triggered) 
	MCUCR |= (1<<ISC11) | (1<<ISC01);

	// turn on interrupts!
	GIMSK  |= (1<<INT1) | (1<<INT0);
	
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