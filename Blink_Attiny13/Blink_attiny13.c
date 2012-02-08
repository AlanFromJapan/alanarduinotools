//
// Test of Attiny13 
// AlanFromJapan http://kalshagar.wikispaces.com/
//
//

// includes
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//
// main function
//
int main(void) {

	//factory settings is to divide internal clock 8MHz by 8.
	//don't, and just run at 8 MHz (set the clock divider to 1 so no effect)
	CLKPR = (1<<CLKPCE);
	CLKPR = 0; // Divide by 1 

	//port B pins 0-5 go OUTPUT
    DDRB = 0xFF;
	
	while (1==1){
		//all on
		PORTB = 0xFF;
		
		//wait a little
		_delay_ms(500);
		
		//all off
		PORTB = 0x00;
		
		//wait again
		_delay_ms(200);
		
	}
}
	