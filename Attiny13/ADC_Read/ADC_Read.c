/*
 * ADC_Read.c
 *
 * Created: 2012/02/12 14:12:03
 *  Author: Alan
 */ 

#define F_CPU 8000000L
 
#include <avr/io.h>
#include <util/delay.h>
 
#define LED PB2 // Define led ext output pin on PB2
 
int i; // 8 bits integer

void setupPWM() {
	/* Set Fast PWM mode. */
	TCCR0A |= (1<<WGM00) | (1<<WGM01);
	/* Clear 0C0A on compare. */
	TCCR0A |= (1<<COM0A1);
	/* Start timer, no prescaling. */
	TCCR0B |= (1<<CS00);
	//initial PWM value is 0
	OCR0A = 0x00;
}


//setup ADC as read on AD2 (PB4) with the max clock prescaler 
void setupADC() {

	ADMUX |= 
		(1 << ADLAR) // AD result store in (more significant bit in ADCH) : ADCH holds the 8 MSB like that: perfect for PWM
		| (1 << MUX1); // Choose AD input AD2 (BP 4)	

	//divide main clock by 128 for better results
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	
}


//this is the QUICK version, returns the high bits only
uint8_t doADC(){
	ADCSRA |= (1 << ADEN); // Analog-Digital enable bit
	ADCSRA |= (1 << ADSC); // Discarte first conversion
		 
	while (ADCSRA & (1 << ADSC)); // wait until conversion is done
		 
	ADCSRA |= (1 << ADSC); // start single conversion
		 
	while (ADCSRA & (1 << ADSC)) // wait until conversion is done
		 
	ADCSRA &= ~(1<<ADEN); // shut down the ADC

	return ADCH;	
}

	
int main(void) {
	uint16_t vADC; 
 	//factory settings is to divide internal clock 8MHz by 8.
	//don't, and just run at 8 MHz (set the clock divider to 1 so no effect)
	CLKPR = (1<<CLKPCE);
	CLKPR = 0; // Divide by 1 

	//port B pins 0-5 go OUTPUT
    DDRB = 0xFF;

	setupPWM();
	
	setupADC();
 
	for (;;) {
		//----------Show ADCH Byte in Led variable brightness indicator---------
		OCR0A = doADC();
		
		_delay_ms(50);
		/*
		for (i = 0 ; i < ADCH ; i++) // Loop x time until i reach ADCH value
		{
		_delay_ms (10); // Loop delay
		}
		*/
 
		//PORTB ^= (1 << LED); // Inverte led bit and show it
	 }
	 return 0;
 }