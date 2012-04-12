/*
 * BalconyMeter.c
 *
 * Created: 2012/04/08 18:33:34
 *  Author: Alan
 */ 

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/sleep.h>

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

#define DOT_MASK 0b00000100

#define AVGBUFFER_LEN 5
uint16_t mTempBufferAverage[AVGBUFFER_LEN];
uint8_t mTempBufferHead = 0;

#define DIGIT_POV_US 1000
#define PINBIT_DECIMAL 2
#define PINBIT_UNITS 3
#define PINBIT_TENS 4


//We consider that all digit given as parameter are x10 compared to the real value (for showing 1 decimal)
// 3 means 0.3, 99 means 9.9, 123  means 12.3
void showDigit(uint16_t pValue){
	//decimals
	PORTD =  mLetters[pValue % 10];
	PORTC =  (~(1 << PINBIT_DECIMAL)) & 0xFC;
	//pov	
	_delay_us(DIGIT_POV_US);
	
	//units + the decimal dot
	PORTD =  mLetters[(pValue / 10) % 10] | DOT_MASK;
	PORTC =  (~(1 << PINBIT_UNITS)) & 0xFC;
	//pov	
	_delay_us(DIGIT_POV_US);
	
	//tens if != 0
	if ((pValue/ 100) % 10 != 0) {
		PORTD =  mLetters[(pValue/ 100) % 10];
		PORTC =   (~(1 << PINBIT_TENS)) & 0xFC;
		//pov	
		_delay_us(DIGIT_POV_US);
	}	
}

void numbers99dot9(){
	
	uint16_t i = 0;
    while(1)
    {
		//just some silly temporisation
		for (uint32_t j = 0; j < 1000; j++){
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

uint16_t ReadADCx16() 
{
	//goto ADC noise reduction mode
	//SMCR = (1 << SM0) | (1 << SE); 	sleep_mode();
	
	
	ADCSRA |= (1 << ADSC); // Start conversion
	while (ADCSRA & (1 << ADSC)); // wait until conversion is done

	uint16_t v = ADCL;
	v = (ADCH << 8) + v;
	

	
	return v;
}

void ReadTemp(){
	
	
    while(1)
    {
		uint32_t v = ReadADCx16();
		//convert to centigrad and *10 (because we are working with INT and want to display 1 decimal)
		//v = (v * 5 * 100 * 10) / 1024; <=> v * 5 * 1000 / 1024 <=> v *5 but we loose a bit of precision
		//v = v * 5; 
		v = (v * 5 * 100 * 10) >> 10; //>> 10 <=> /1024
		
		//ad'hoc calibration
		//apparently my system reads a bit too high temp, about 5% too high
		//I don't have the will to make extensive test, calculate formula etc... 
		v = v - v/20;
		
		//average the temp with latest readings
		mTempBufferAverage[mTempBufferHead] = v;
		mTempBufferHead = (mTempBufferHead +1) % AVGBUFFER_LEN;
		
		//calculate an average with a circle buffer
		v= 0;
		for (uint32_t j = 0; j < AVGBUFFER_LEN; j++){
			v += mTempBufferAverage[j];
		}
		v = v / AVGBUFFER_LEN;
		
		//just some silly temporisation
		for (uint32_t j = 0; j < 150; j++){
			showDigit(v);
		}
		
		
    }	
}




int main(void)
{
	//uses a 12MHz crystal because I messed the fuses ... so let's go slowly and run 1/256th of that speed to save electricity
	CLKPR = (1 << CLKPCE) | (1 << CLKPS3);
	
	//PORTD is the 7segment control, everybody goes output
	DDRD = 0xFF;
	
	//PORTC bits [5-2] control which 7segment is selected
	//bits [1-0] are for input of temperature
	DDRC = 0b11111100;
	
	//start with all segments off
	PORTC = 0x00;
	
	
/*********************************************************************/
	//ADC setup
	//ADC0, right adjust, using aref
	//ADMUX = 0x00;
	ADMUX = 0x00 | (1 << REFS0) | (0 << ADLAR);
	
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
	
/*********************************************************************/	
	//SimpleTestDigit();
	
	//numbers99dot9();
	
    //TestAllSegments();

	ReadTemp();
}