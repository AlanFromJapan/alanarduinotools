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

//Buttons are on PORTB
#define BUTTON_A	0x01
#define BUTTON_B	0x02
#define BUTTON_X	0x04
#define BUTTON_Y	0x08
#define BUTTON_ZL	0x10

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
	
void testRGB(){
	uint8_t v = 0;
	
	while (1){
		_delay_ms(1000);
		v += 1;
		
		uint8_t vrgb = v << 2;
		
		PORTD &= 0b11100011;
		PORTD |= vrgb;
		
		if (v >= 0x07){
			v = 0;
		}
	}
	
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
			SaySomething("kira-mo-do.", 0x01);
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

	
int main(void)
{
	AquestLSI_Init();
	setupButtons();
	setupRGB();
	
	setMode(mCurrentMode);
	
    while(1)
    {
		//BUTTON A : CHANGE MODE
		if ((PINB & BUTTON_A) == 0x00) {
			uint8_t vMode = (mCurrentMode + 1) % MODE_COUNT;
			setMode(vMode);
		}
		
    }
}