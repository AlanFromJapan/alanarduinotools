/*
 * TinyVoltmeter.c
 *
 * Created: 2012/03/27 23:25:56
 *  Author: Alan - http://kalshagar.wikispaces.com/Tiny+Voltmeter
 */ 

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/pgmspace.h>


#define VOLTAGE_SCALE_FACTOR 2

 uint8_t mLetters[] = {
	0b00001000,//0 positive
	0b01011011,//1 positive
	0b00100010,//2 positive
	0b00010010,//3 positive
	0b01010001,//4 positive
	0b00010100,//5 positive
	0b00000100,//6 positive
	0b01011010,//7 positive
	0b00000000,//8 positive
	0b00010000//9 positive
};



void showDigit(uint8_t pValue){
	//units
	PORTA =  mLetters[pValue % 10];
	PORTB =  (1 << 5);
	//pov	
	_delay_us(1000);
	
	//tens
	PORTA =  mLetters[(pValue/ 10) % 10];
	PORTB =  (1 << 4);
	//pov	
	_delay_us(1000);
}


uint8_t ReadADCx() 
{
	
	ADCSRA |= (1 << ADSC); // Start conversion
	while (ADCSRA & (1 << ADSC)); // wait until conversion is done

	uint8_t v = ADCH;
	return v;
}

uint16_t ReadADCx16() 
{
	
	ADCSRA |= (1 << ADSC); // Start conversion
	while (ADCSRA & (1 << ADSC)); // wait until conversion is done

	uint16_t v = ADCL;
	v = (ADCH << 8) + v;
	return v;
}

int main(void)
{
	//Timer full speed on internal oscillator !
	CLKPR = (1<<CLKPCE);
	CLKPR = 0; // Divide by 1 
	
	//all port A goes output 
	DDRA = 0xFF;
	//port B goes output save adc9/PB6
	DDRB = 0b10111111;
	
	
	
/*	
	PORTA = 0b00001000;//0 positive
	PORTA = 0b01011011;//1 positive
	PORTA = 0b00100010;//2 positive
	PORTA = 0b00010010;//3 positive
	PORTA = 0b01010001;//4 positive
	PORTA = 0b00010100;//5 positive
	PORTA = 0b00000100;//6 positive
	PORTA = 0b01011010;//7 positive
	PORTA = 0b00000000;//8 positive
	PORTA = 0b00010000;//9 positive
	
	PORTB =  (1 << 4) | (1<< 5);
*/	

/*	
	uint8_t i = 0;
    while(1)
    {
        PORTA =  mLetters[i];
		PORTB =  (1 << 4);
		
		i= (i + 1) % 10;
		_delay_ms(10000);
    }
*/

	//------------START ADC INIT --------------------------
	ADCSRA |= (1 << ADEN)  // Analog-Digital enable bit
	
	| (1 << ADPS1) // set prescaler to 8 (clock / 8)
	| (1 << ADPS0) // set prescaler to 8 (clock / 8)
	
//	| (1 << REFS1) //1.1v internal ref
//	| (1 << REFS2) | (1 << REFS1) //2.56v internal ref, without AREF
	| (1 << REFS1) | (0 << REFS0) //2.56v internal ref, without AREF
	;
	ADCSRB |= 
	(1 << REFS2) //2.56v internal ref, without AREF
	| (0 << GSEL) //no gain
	| (0 << MUX5) //adc9 = 001001
	;
	
	ADMUX |= 
//	(1 << ADLAR) //just read ADH 	
	(0 << ADLAR) //read ADL then ADH 
	| (1 << MUX3) | (1 << MUX0) //adc9 = 001001
	;
	//------------END ADC INIT --------------------------

	uint16_t i = 0;
    while(1)
    {
		
//		i = ReadADCx();//i in [0;255]
		i = ReadADCx16();//i in [0;1024]
		
		//adjustement : my readings are a little to high (+10%)
		//also readings [0:1024]->[0:2.56v] and since voltage is measured through a divider scale, adjust
		i = i * VOLTAGE_SCALE_FACTOR - VOLTAGE_SCALE_FACTOR * i /10;
		
		//uint16_t i50 = (uint16_t)i * (uint16_t)50 / (uint16_t)255;
//		uint16_t i50 = (uint16_t)i * (uint16_t)50; i50 = i50 >> 8; //same as /255
		uint16_t i50 = (uint16_t)i * (uint16_t)50; i50 = i50 >> 10; //same as /1024
		
		//just some silly temporisation
		for (uint32_t j = 0; j < 1500; j++){
			showDigit(i50);
		}
    }
	
}
