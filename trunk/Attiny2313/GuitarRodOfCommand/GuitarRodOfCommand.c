/*
 * GuitarRodOfCommand.c
 *
 * Created: 2013/01/13 11:56:49
 *  Author: Alan
 */ 


//TWI is made to work at 1MHz, same for the timings of the IR. 
//So if you change the speed of you uC, you'll have to check and most likely change a thing or two.
#define F_CPU 1000000UL	      // Sets up the default speed for delay.h

#include <avr/io.h>
#include <util/delay.h>
#include "IR/IR_Sharp.h"
#include "IR/IR_NEC.h"

//Buttons are on PORTB
#define BUTTON_A	0x01
#define BUTTON_B	0x02
#define BUTTON_X	0x04
#define BUTTON_Y	0x08
#define BUTTON_ZL	0x10
#define BUTTON_ACTION_MASK (BUTTON_B | BUTTON_X | BUTTON_Y | BUTTON_ZL)

//Different modes
#define MODE_KILLER	0x00
#define MODE_TV		0x01
#define MODE_VCR	0x02
#define MODE_PC		0x03
#define MODE_COUNT	4
uint8_t mCurrentMode = MODE_KILLER;

void setupButtons(){
	
	//B0-B4 are the buttons (A-B-X-Y-ZL)
	//They are grounded when closed, so we need to pullup the buttons (internal pullups)

	//B goes input, with pullups : care for the order DDRx -> PORTx -> MCUCR
	DDRB = 0xE0; //0b11100000 B7-B5 : TWI I2C, B4-B0 : buttons
	PORTB = 0x1F; //pull ups
	MCUCR |= (0 << PUD); //make sure pullups are not disabled
}
	
//RGB led on port D2-4 (R-B-G)
void setupRGB(){
	DDRD |= 0x1C; //0b00011100
}

//D6 is the IR	
void setupIR() {
	//IR pin is D6 as output
	DDRD |= 0b01000000;
}

//Color are 0b000RBG00
void setColor (uint8_t pColor){
	PORTD &= 0b11100011;
	PORTD |= pColor;
}
	
void setMode (uint8_t pMode){
	mCurrentMode = pMode;
	
	switch(mCurrentMode){
		case MODE_KILLER:
			setColor(0x10); //RED
			SaySomething("pawa-mo-do.", 0x01);
			break;
		case MODE_TV:
			setColor(0x08); //BLUE
			SaySomething("terebi-.", 0x01);
			break;
		case MODE_VCR:
			setColor(0x04); //GREEN
			SaySomething("bishiaru.", 0x01);
			break;
		case MODE_PC:
			setColor(0x18); //PURPLE
			SaySomething("pasokon.", 0x01);
			break;
	}
}

//What to do when click a button in mode KILLER
void doAction_Killer (uint8_t pButton){
	//save color
	uint8_t vOldPORTD = PORTD;
	
	switch(pButton){
		case BUTTON_B:
			//TV Sharp ON/OFF
			IRSendSharp (0x41A2, 15);
			break;
		case BUTTON_X:
			//VCR Buffalo ON/OFF
			IRSendNEC(0x30EE, 0xB1);
			break;		
	}
	
	//reset to previous value
	PORTD = vOldPORTD;
}


//What to do when click a button in mode TV
void doAction_TV (uint8_t pButton){
	//save color
	uint8_t vOldPORTD = PORTD;
	
	switch(pButton){
		case BUTTON_B:
		//TV Sharp Input change
		IRSendSharp (0x4322, 15);
		break;
		case BUTTON_X:
		//TV Sharp VOLUME+
		IRSendSharp (0x40A2, 15);
		break;
		case BUTTON_Y:
		//TV Sharp VOLUME-
		IRSendSharp (0x42A2, 15);
		break;
		case BUTTON_ZL:
		//nothing
		break;
	}
	
	//reset to previous value
	PORTD = vOldPORTD;
}

//What to do when click a button in mode VCR
void doAction_VCR (uint8_t pButton){
	//save color
	uint8_t vOldPORTD = PORTD;
	
	switch(pButton){
		case BUTTON_B:
		//Play/Pause
		IRSendNEC(0x30EE, 0x8B);
		break;
		case BUTTON_X:
		//List of records
		IRSendNEC(0x30EE, 0xC7);
		break;
		case BUTTON_Y:
		//UP
		IRSendNEC(0x30EE, 0x39);
		break;
		case BUTTON_ZL:
		//DOWN
		IRSendNEC(0x30EE, 0x43);
		break;
	}
	
	//reset to previous value
	PORTD = vOldPORTD;
}
	
int main(void)
{
	AquestLSI_Init();
	setupButtons();
	setupRGB();
	setupIR();
	
	//set current mode
	setMode(mCurrentMode);	
	
    while(1)
    {
		//BUTTON A : CHANGE MODE
		if ((PINB & BUTTON_A) == 0x00) {
			uint8_t vMode = (mCurrentMode + 1) % MODE_COUNT;
			setMode(vMode);
		}
		
		
		//Action button pressed (B-X-Y-ZL)		
		//Check if ANY one is pressed : /PINB != 0
		if ((~(PINB) & BUTTON_ACTION_MASK) != 0x00){
			uint8_t vButton = 0x00;
			
			//get which button was pressed 
			vButton = ((PINB & BUTTON_B) == 0x00 ? BUTTON_B : vButton);
			vButton = ((PINB & BUTTON_X) == 0x00 ? BUTTON_X : vButton);
			vButton = ((PINB & BUTTON_Y) == 0x00 ? BUTTON_Y : vButton);
			vButton = ((PINB & BUTTON_ZL) == 0x00 ? BUTTON_ZL : vButton);
			
			switch(mCurrentMode){
				case MODE_KILLER:
					doAction_Killer(vButton);
					break;
				case MODE_TV:
					doAction_TV(vButton);
					break;
				case MODE_VCR:
					doAction_VCR(vButton);
					break;
			}
			
			//debouncing on the cheap
			_delay_ms(500);
		}
    }
}