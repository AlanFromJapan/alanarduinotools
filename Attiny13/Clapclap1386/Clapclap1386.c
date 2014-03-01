/*
 * Clapclap1386 (ATtiny13 and LM386 version)
 *
 * Created: 2014/03/01 1:16:27
 *  Author: Alan
 *  http://kalshagar.wikispaces.com/Clapclap1386
 *
 */ 

#define F_CPU 4800000L

#define CLAP_GAP_IN_MS 350
#define CLAP_GAP_TOLERANCE 200

#define CLAP_GAP_MIN (CLAP_GAP_IN_MS - CLAP_GAP_TOLERANCE)
#define CLAP_GAP_MAX (CLAP_GAP_IN_MS + CLAP_GAP_TOLERANCE)

#define PORT_LED_CLAP PORTB3
#define PORT_LED_STATUS PORTB4
#define PORT_RELAY PORTB2

//Clock is internal 4.8 MHz, TIM0 is 8bits,  TIM0 prescaler is 64 so :
// Freq of TIM0 interrupt = (48.10^5 / 256) / 64 ~= 293 Hz
#define TIM0_OVF_FREQ 293
//Soft scaler : count only one out of TIM0_OVF_SOFTSCALER interrupt
//This is a *gross* estimation (TIM0_OVF_FREQ/1000), we won't count exact millisecond, we should change the prescaler to fall
//on whole figures but : (1) accuracy is *not* necessary here and (2) it gives more time between interrupts.
#define TIM0_OVF_SOFTSCALER 3

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

volatile uint32_t mTimeCounterMSec = 0;
volatile uint32_t mTimeLastClap = 0;
volatile uint8_t mTimerOverflowCounter = 0;


//Just for the test of leds ... will be removed by optimizer
void test_Blink() 
{
	
	//all out
	//DDRB = 0xFF;
		
    while(1)
    {
        _delay_ms(500);
		
		PORTB = 1 << PORT_LED_CLAP;
		
		_delay_ms(500);
		
		PORTB = 1 << PORT_LED_STATUS;
		
    }
}



//Blink on clap
void blink_clap(){
	PORTB |= (1 << PORT_LED_CLAP); //led on
	_delay_ms(10);
	PORTB &= ~(1 << PORT_LED_CLAP); // led off
}


//Toggle status led and the relay
void blink_doubleClapDetected(){
	PORTB ^= (1 << PORT_RELAY); //toggle relay
	PORTB ^= (1 << PORT_LED_STATUS); //toggle status led	
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

//Interrupt code executed when the timer0 (8 bits) overflows
ISR(TIM0_OVF_vect){
	mTimerOverflowCounter++;
	if (mTimerOverflowCounter >= TIM0_OVF_SOFTSCALER){
		mTimerOverflowCounter=0;
		
		//do the job: count the time (don't care if it roll-over to 0)
		mTimeCounterMSec++;
	}	
}

//inits timer 0 to do interrupt on overflow (calls ISR(TIMER0_OVF_vect))
void init_timer0_OVF() {
	//timer 0 prescaler : 64 -> interrupt trigered at every 256* 64 clock tick ~= 293 Hz
	//(because it's a 8bit OVERFLOW interrupt with a prescaler of 64)	
	TCCR0B= (1 << CS01) | (1 << CS00);
	
	//trigger the interrupt vector TIMER0_OVF_vect when timer 1 is overflow
	TIMSK0 = (1 << TOIE0);
	
	//sets the interruptions to enabled
	sei();
}

int main(void)
{
	//no diviser on clock
	CLKPR = (0<<CLKPCE);
	
	//all pins are output EXCEPT PB0 & PB1 (AIN0 and AIN1) that are input
	DDRB = 0xFF 
		& ~(1 << PB0) 
		& ~(1 << PB1);
	
	//needed it seems.
	//One need also a pulldown on the AIN0 (Comparator+) to avoid having it floating (if you use a switch for instance)
	PORTB &= ~(1<<PB0);    // no Pull-up on PB0
	
	//relay and status led are OFF
	PORTB &= ~(1 << PORT_RELAY) & ~(1 << PORT_LED_STATUS);
		
	//test_Blink();
	ACSR  |=  (1<<ACI);    // clear Analog Comparator interrupt
	ACSR  |=
	(0<<ACD)   |         // Comparator ON
	(0<<ACBG)  |         // Disconnect 1.23V reference from AIN0 (use AIN0 and AIN1 pins)
	(1<<ACIE)  |         // Comparator Interrupt enabled
//	(0<<ACIC)  |         // input capture disabled
	(1<<ACIS1) |         // set interrupt bit on rising edge
	(1<<ACIS0);          //    (ACIS1 and ACIS0 == 11)	
	
	sei();  // enable global interrupts	

	//start the Timer0 8bit timer to count (roughly) the milliseconds
	init_timer0_OVF();

	//main body loop : do nothing and let the interrupts do the job
	while(1) {}
}