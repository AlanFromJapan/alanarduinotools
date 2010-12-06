//
// Home Presence Detector
// AlanFromJapan http://kalshagar.wikispaces.com/
//

// includes
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//use this trick to ""adjust"" the timer1 and subscale it
#define TIMER_DIVIDER 2
volatile uint16_t mTimerCounter = 0;
volatile unsigned char mPinDIn;

ISR(TIMER1_OVF_vect){
	mTimerCounter++;
	
	if (mTimerCounter >= TIMER_DIVIDER){
		//first thing, reset
		mTimerCounter = 0;
		//then do the interrupt job
		
		//read input on PB7, and light or stop D port
		mPinDIn = PIND;
		if (!(mPinDIn & 0x01)){
			//movement detected!
			PORTB |= 0x80;
		}
		else {
			//movement off
			PORTB &= 0x7F;
		}
	}
}

//inits timer 1 to do interrupt on overflow (calls ISR(TIMER1_OVF_vect))
void init_timer1_OVF() {
	//timer 1 prescaler : makes clock / 64  -> tick every 1/4 sec roughly
    TCCR1B=0x03;
	//trigger the interrupt vector TIMER1_OVF_vect when timer 1 is overflow
    TIMSK = 0x80;

	//sets the interruptions to enabled
	sei();
}


//Inits timer 0 to do PWM on pin B2
void init_timer0_PWM()
{
	/* Set Fast PWM mode. */
	TCCR0A |= (1<<WGM00) | (1<<WGM01);

	/* Clear 0C0A on compare. */
	TCCR0A |= (1<<COM0A1);

	/* Start timer, no prescaling. */
	TCCR0B |= (1<<CS00);

	/* Duty cycle 0% */
	OCR0A = 0;
}



//
// main function
//
int main(void) {

	//factory settings is to divide internal clock 8MHz by 8.
	//don't, and just run at 8 MHz (set the clock divider to 1 so no effect)
	CLKPR = (1<<CLKPCE);
	CLKPR = 0; // Divide by 1 

	//ports of bank D goes Mixed : PD0 in (movement detected on low), others out
	DDRD = 0xFE;

	//all ports of bank B goes output PB6~0 is output
	DDRB = 0xFF;

	//will be used to re-divide the TIMER1 (too fast)
	mTimerCounter = 0;

	//init for PWM
	init_timer0_PWM();
	//init for timer interrupt
	init_timer1_OVF();

	unsigned char vIn;
	
	while (1){
	
		//read input on PB7, and light or stop D port
		vIn = PIND;
		if (!(vIn & 0x01)){
			//movement detected!
			PORTB |= 0x80;
		}
		else {
			//movement off
			PORTB &= 0x7F;
		}
	
		
		/*
		//pulse led
		unsigned char i ;
		for(i=0; i < 255; i++) {
			OCR0A = i;
			_delay_ms(10);
		}

		for(i=255; i > 0; i--) {
			OCR0A = i;
			_delay_ms(10);
		}		
		OCR0A = 0;
		*/
		
		_delay_ms(50);
	}

	/*
	//test : blink the port D
	PORTD = 0x7F;
	while (1){
		PORTD ^= 0x7F;
		_delay_ms(3000);
	}
	*/ 
}


////////////////////////////////////////////////////////////