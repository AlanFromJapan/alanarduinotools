//
// Clapclap (Attiny2313 version)
// AlanFromJapan http://kalshagar.wikispaces.com/
//
// Main source about the comparator usage on ATtiny2313 : http://www.tvbgone.com/mfaire/bugbot/bugbot.c
//

// includes
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


//B3 is 0x08 = b00001000
void test_blinkB3(){
	PORTB ^= 0x08; //led on
	_delay_ms(50);
	PORTB ^= 0x08; // led off
}


//B2 is 0x04 = b00000100
void test_blinkB2(){
	PORTB ^= 0x04; //led on
	_delay_ms(100);
	PORTB ^= 0x04; // led off
}


//Interrupt code of the comparator
// Our settings is interrupt on rising edge. Meaning no 2 consequent interrupt if stays high. It is rose when comparator output goes from 0 to 1 only.
// NB: AIN0 is POSITIVE pin, AIN1 is NEGATIVE pin
ISR(ANA_COMP_vect) {

	//with this test, ensure that indeed the interrupt result is 1 = V(AIN0) > V(AIN1)
	//(double check ?)
	if ( (ACSR & (1 << ACO)) != 0 ) {
		test_blinkB2();	
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
  
	while (1) {/*all the job is done in the interrupt */
		// a little "is alive" debug trick that blink another led every second or so
		test_blinkB3(); _delay_ms(1000);
	}
}


////////////////////////////////////////////////////////////