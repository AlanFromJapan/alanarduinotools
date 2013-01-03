/*
 * IR_Sharp.h
 *
 * Created: 2013/01/03 20:11:53
 *  Author: Alan
 * Based on the IRremote lib for Arduino and the great work done on : http://www.sbprojects.com/knowledge/ir/sharp.php
 * Codes for sharp : http://lirc.sourceforge.net/remotes/sharp/GA538WJSA
 * 
 */ 


#ifndef IR_SHARP_H_
#define IR_SHARP_H_

//Fix the F_CPU BEFORE including delay.h for correct timing
#define F_CPU 1000000UL

#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

//Durations in MICRO Sec
//The hard coded adjustment are for the execution of the code itself that spoils the timing (valid for 1MHz F_CPU)
#define SHARP_BIT_MARK 320
#define SHARP_ONE_SPACE (2000 - SHARP_BIT_MARK -50)
#define SHARP_ZERO_SPACE (1000 - SHARP_BIT_MARK -50)
#define SHARP_GAP 600000
#define SHARP_TOGGLE_MASK 0x3FF
#define SHARP_RPT_SPACE 3000

//Using a 1/4 duty, this is the time High, the time low and the total time
//The time high and low have 4us discounted : with a 1MHz speed that's the payload for difference between theory and practice
#define SHARP_PULSE_HIGH_US (6.58 -4.0)
#define SHARP_PULSE_LOW_US (19.74 -4.0)
#define SHARP_PULSE_FULL_PERIOD (26.32)


//Send the pulse at 38kHz for pMicroSec duration
void mark (double pDurationUS){
	//how many time shall we send the pulse over the period (rounded up)
	uint16_t vCount = 1+ pDurationUS / SHARP_PULSE_FULL_PERIOD;
	
	for(uint16_t i = 0; i < vCount; i++){
		//on
		PORTD = 0xFF;
		_delay_us(SHARP_PULSE_HIGH_US);
	
		//off
		PORTD = 0x00;
		_delay_us(SHARP_PULSE_LOW_US);
	}	
}

inline void space (uint16_t pDurationUS){
	//Low
	//off
	PORTD = 0x00;
	_delay_us(pDurationUS);
}

/* Sharp and DISH support by Todd Treece ( http://unionbridge.org/design/ircommand )

The Dish send function needs to be repeated 4 times, and the Sharp function
has the necessary repeat built in because of the need to invert the signal.

Sharp protocol documentation:
http://www.sbprojects.com/knowledge/ir/sharp.htm

Here are the LIRC files that I found that seem to match the remote codes
from the oscilloscope:

Sharp LCD TV:
http://lirc.sourceforge.net/remotes/sharp/GA538WJSA

*/

void IRSendSharp(uint16_t data, uint8_t nbits) {
	uint16_t invertdata = data ^ SHARP_TOGGLE_MASK;
	
	for (uint8_t i = 0; i < nbits; i++) {
		if (data & 0x4000) {
			mark(SHARP_BIT_MARK);
			space(SHARP_ONE_SPACE);
		}
		else {
			mark(SHARP_BIT_MARK);
			space(SHARP_ZERO_SPACE);
		}
		data <<= 1;
	}
	
	//Exp
	//mark(SHARP_BIT_MARK);
	//space(SHARP_ONE_SPACE);
	//Chk
	mark(SHARP_BIT_MARK);
	space(SHARP_ZERO_SPACE);
	
	//delay
	_delay_ms(40);
	
	//repeat
	for (uint8_t i = 0; i < nbits; i++) {
		if (invertdata & 0x4000) {
			mark(SHARP_BIT_MARK);
			space(SHARP_ONE_SPACE);
		}
		else {
			mark(SHARP_BIT_MARK);
			space(SHARP_ZERO_SPACE);
		}
		invertdata <<= 1;
	}
	mark(SHARP_BIT_MARK);
	space(SHARP_ZERO_SPACE);
	//_delay_ms(46);
}




#endif /* IR_SHARP_H_ */