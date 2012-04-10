/*
 * BalconyMeter.c
 *
 * Created: 2012/04/08 18:33:34
 *  Author: Alan
 */ 

#include <avr/io.h>
#include <avr/delay.h>

 uint8_t mLetters[] = {
	0b11101011,//0 positive
	0b10001000,//1 positive
	0b10110011,//2 positive
	0b10111010,//3 positive
	0b11011000,//4 positive
	0b01111010,//5 positive
	0b01111011,//6 positive
	0b10101000,//7 positive
	0b11111011,//8 positive
	0b11111010//9 positive
};

uint8_t mDotMask = 0b00000100;

void showDigit(uint16_t pValue){
	//decimals
	PORTD =  mLetters[pValue % 10];
	PORTC =  ~(1 << 2);
	//pov	
	_delay_us(1000);
	
	//units
	PORTD =  mLetters[(pValue / 10) % 10] | mDotMask;
	PORTC =  ~(1 << 3);
	//pov	
	_delay_us(1000);
	
	//tens
	PORTD =  mLetters[(pValue/ 100) % 10];
	PORTC =  ~(1 << 4);
	//pov	
	_delay_us(1000);
}

void numbers99dot9(){
	
	uint16_t i = 0;
    while(1)
    {
		//just some silly temporisation
		for (uint32_t j = 0; j < 1500; j++){
			showDigit(i);
		}
		
		i= (i + 1) % 1000;
    }	
}

void TestAllSegments() 
{
	while(1)
    {
		//j selects the number
        for (int j = 4; j >= 2; --j){
			//start with all segments off
			PORTC = 0x00;
			PORTC = ~(1 << j);
			
			for (int i=0; i < 8; i++){
				PORTD = (1 << i);
				
				_delay_ms(8);
			}
		}
    }
}



int main(void)
{
	//factor settings is to divide internal clock 8MHz by 8.
    //don't, and just run at 8 MHz (set the clock divider to 1 so no effect)
    CLKPR = (1<<CLKPCE);
    CLKPR = 0; // Divide by 1 
	
	//PORTD is the 7segment control, everybody goes output
	DDRD = 0xFF;
	
	//PORTC bits [5-2] control which 7segment is selected
	//bits [1-0] are for input of temperature
	DDRC = 0b11111100;
	
	//start with all segments off
	PORTC = 0x00;
	
	//SimpleTestDigit();
	
	numbers99dot9();
	
    //TestAllSegments();

}