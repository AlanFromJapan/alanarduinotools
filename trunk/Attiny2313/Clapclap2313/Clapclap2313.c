//
// Clapclap (Attiny2313 version)
// AlanFromJapan http://kalshagar.wikispaces.com/Clapclap2313
//
// Main source about the comparator usage on ATtiny2313 : http://www.tvbgone.com/mfaire/bugbot/bugbot.c
//

// includes
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//#define F_CPU 8000000L //already in the makefile
#define CYCLE_PER_MS (F_CPU / 1000L)

//this mystery divider is obtained via direct measures. No idea why this value comes out, can't find a logical explanation (counting cycles of my code?).
//it makes something like 13,333 cycles per second. I don't need precise measures, so it's ok, but still it's a shame...
#define MYSTERY_DIVIDER_SEC (8000UL * 2UL * 50UL / 60UL)
#define MYSTERY_DIVIDER_MILISEC (8UL * 2UL * 50UL / 60UL)

#define CLAP_GAP_IN_MS 350
#define CLAP_GAP_TOLERANCE 200

#define CLAP_GAP_MIN (CLAP_GAP_IN_MS - CLAP_GAP_TOLERANCE)
#define CLAP_GAP_MAX (CLAP_GAP_IN_MS + CLAP_GAP_TOLERANCE)


volatile uint32_t mTimeCounterMSec = 0;
volatile uint32_t mTimeLastClap = 0;

//B3 is 0x08 = b00001000
void blink_clap(){
	PORTB ^= (1 << 3); //toggle led
	PORTD ^= (1 << 4); //toggle led
	_delay_ms(10);
	PORTB ^= (1 << 3); // toggle led
	PORTD ^= (1 << 4); //toggle led
}


//Blink led is on B2 0x04 = b00000100 = 0x04 = (1 << 2)
//Relay is on B4 = b00010000 = 0x10 = (1 << 4)
void blink_doubleClapDetected(){
	PORTB ^= (1 << 4); //toggle relay

	PORTB ^= (1 << 2); //toggle led
	PORTD ^= (1 << 5); //toggle led
	_delay_ms(10);
	PORTB ^= (1 << 2); // toggle led
	PORTD ^= (1 << 5); //toggle led
}


//Interrupt code of the comparator
// Our settings is interrupt on rising edge. Meaning no 2 consequent interrupt if stays high. It is rose when comparator output goes from 0 to 1 only.
// NB  : AIN0 is POSITIVE pin, AIN1 is NEGATIVE pin
// NB2 : you need to stabilize the power rail with a capacitor (otherwise you'll have ripples and misreading). With AIN1=Comp-=0v a 200uF is fine.
ISR(ANA_COMP_vect) {

	//with this test, ensure that indeed the interrupt result is 1 = V(AIN0) > V(AIN1)
	//(double check ?)
	if ( (ACSR & (1 << ACO)) != 0 ) {
	
		blink_clap();	
		
		volatile uint32_t vNow = mTimeCounterMSec;
		volatile uint32_t vGap = vNow - mTimeLastClap;
		
		if (
			mTimeLastClap != 0UL
			&& mTimeLastClap < vNow
			&& vGap >= CLAP_GAP_MIN && vGap <= CLAP_GAP_MAX
			) {
		
			mTimeLastClap = 0UL;
			blink_doubleClapDetected();
		}
		else {
			mTimeLastClap = vNow;
		}
	}

}

//Test purpose. This pulses 1 per second
//Sorry, no idea why the math work here ... that puzzles me.
void test_pulse1perSecond(){
	uint32_t v = 0;
	
	while(1) {
		v++;
			
		//do NOT ask me why with this formula ! get 1 pulse per sec, but it's a fact...
		//the final 50/60 was made from direct measures (how many pulses on a minute)
		if ((v % MYSTERY_DIVIDER_SEC) == 0L){
		
			blink_clap();
			v=0;
		}
	}
	
	//this means 1 second is 13,333 cycles... look like the thing runs at 13 kHz oO; ??
}


//Counts seconds
void counterSeconds(){
	volatile uint32_t v = 0UL;
	
	while(1) {
		v++;
			
		//do NOT ask me why with this formula ! get 1 pulse per sec, but it's a fact...
		//the final 50/60 was made from direct measures (how many pulses on a minute)
		if ((v % MYSTERY_DIVIDER_MILISEC) == 0UL){
			mTimeCounterMSec++;
			
			if ((v % 1000UL) == 0UL){
				v=0UL;
			}	
		}
	}
}

//
// main function
//
int main(void) {

	//factory settings is to divide internal clock 8MHz by 8.
	//don't, and just run at 8 MHz (set the clock divider to 1 so no effect)
	CLKPR = (1<<CLKPCE);
	CLKPR = 0; // Divide by 1 

	//port B pins 7-2 go output, 1-0 go input(it's the comparator)
    DDRB = 0xFC;
	//On the final stuff I'll use pins on row D, sol let's turn them output too
	DDRD = 0xFF;
	
	//needed it seems. 
	//One need also a pulldown on the AIN0 (Comparator+) to avoid having it floating (if you use a switch for instance)
	PORTB &= ~(1<<PB0);    // no Pull-up on PB0



	ACSR  |=  (1<<ACI);    // clear Analog Comparator interrupt
	ACSR  |=
		(0<<ACD)   |         // Comparator ON
		(0<<ACBG)  |         // Disconnect 1.23V reference from AIN0 (use AIN0 and AIN1 pins)
		(1<<ACIE)  |         // Comparator Interrupt enabled
		(0<<ACIC)  |         // input capture disabled
		(1<<ACIS1) |         // set interrupt bit on rising edge
		(1<<ACIS0);          //    (ACIS1 and ACIS0 == 11)
		
	
	sei();                 // enable global interrupts

	counterSeconds();
}


////////////////////////////////////////////////////////////
