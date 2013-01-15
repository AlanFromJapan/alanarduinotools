/*
 * IR_NEC.h
 *
 * Created: 2013/01/02 19:03:25
 *  Author: Alan
 */ 


#ifndef IR_NEC_H_
#define IR_NEC_H_

#include "IR_Shared.h"

//Source : http://openlgtv.org.ru/wiki/index.php/Using_a_generic_microcontroller_board_as_an_LG_service_remote

// ====================================================================
// "NEC protocol" IR Remote implementation for the Atmel ATtiny2313
// microcontroller. Originally made for a board with 8 red LEDs and
// two momentary push buttons but only really requires a single LED
// if you reduce the code down to its core.
// --------------------------------------------------------------------
// This should be pretty easily portable to other microcontrollers
// or similar low-end singletasking systems. You need to be able to
// generate accurate delays in the microseconds range, though.
// ====================================================================
//
// Copyright (c) 2011 Jukka Aho <jukka.aho@iki.fi>
//
// Permission is hereby granted, free of charge, to any person
// obtaining a  copy of this software and associated documentation
// files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software,
// and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// The number in the F_CPU macro needs to match your Atmel
// microcontroller CPU clock in Hz. If it doesn't, the delay functions
// in delay.h will not work properly and you will not get proper timing.
// I'm using a 10 MHz crystal with the ATtiny2313, hence 10000000UL.

//AlanFromJapan : Ok I'm using a 1MHz not 10, so change that value and fix all timers
#define F_CPU 1000000UL

//AlanFromJapan : adjust the theorical perfect 1MHz oscillator and the cheap one in your 2313
//In my case, it's a bit fast by 5%
#define CPU_ADJUST 0.95

#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

// On the ATtiny2313 board I used (kudos to my little brother for
// building it and lending it to me in the first place!) there is
// a straight row of 8 red LEDs, looking like this:
//
//                            o o o o o o o o
//                            7 6 5 4 3 2 1 0
//
// Each LED is connected to the I/O lines of the port "B", which is
// represented on the CPU side as an 8-bit, 8-line GPIO register.
// The register that controls these lines is called "PORTB". The
// individual bits of this register directly control whether the
// individual LEDs are lit or not.
//
// The row of LEDs serves dual purposes in this implementation:
//
//  1) The LEDs are used as indicator lights for displaying the
//     currently selected, "active" IR command to the user. (The
//     user can cycle through a small set of predefined, hardcoded
//     IR commands with one push-button, then transmit the selected
//     IR command with a press of another button.)
//
//  2) The LEDs are also used as "IR" LEDs: for blinking/flashing
//     out the IR code when the user presses the "transmit" button.
//
// The latter part works because red LEDs, when lit, will emit some
// energy also in the IR part of the spectrum: not much, but enough
// to control the TV from a close distance. You just need to bring
// the LEDs as near to the IR eye of the TV as possible (5...15 cm
// would appear to be near enough.)
//
// Using a genuine IR LED would give you more range. And even a
// single red LED would do for transmitting the IR commands - you
// don't need 8 of them! But since the board I used was already
// populated with 8 red LEDs and since they handily doubled as
// both IR transmitters and indicator lights, the setup was good
// enough for my purposes, "as is".
//
// Here's a function-like macro for setting the bits of the PORTB
// register: in other words, the state of the indicator LEDs:

//#define set_portb(value) (PORTD = value)

// For quick porting, you can replace this with an actual function or
// another macro definition which deals with the indicator lights in
// a different way. Or if you don't use or need indicator LEDs at all,
// simply redefine set_portb() as a no-op operation, such as a mere
// semicolon. Note that the state of the LEDs connected to PORTB is
// defined in inverse logic: 0 = on, 1 = off.

// As for blinking out the IR commands, we actually set _all_ the
// eight LEDs on and off "in unison":



// If you do not use this dual-purpose LED approach but have a
// separate IR LED for blinking the code out, redefining the
// macros set_ir_led_on() and set_ir_led_off() to something
// sensible for your architecture would do the trick there.

// The board I used has two push-buttons (momentary switches) for
// user input: "SELECT" and "TRANSMIT". They could look like this:
//
//                           [o] SELECT
//
//                           [o] TRANSMIT
//
// In this implementation, we predefine 8 hardcoded IR command codes.
// These will be stored in the flash memory our "remote". The user can
// cycle through the available commands with the "SELECT" button (the
// corresponding indicator LED will be lit) and then press the
// "TRANSMIT" button in order to blink the selected command out.


// This IR Remote implementation was created primarily for getting
// into the service menus of LG TVs.
//
// LG uses the so-called NEC IR protocol in their TVs. This protocol
// transmits a device address byte followed by a bit-inverted
// "checksum" and then a command byte followed by another bit-inverted
// "checksum". That's about it: there are four bytes in all.
//
// The TVs manufactured by LG respond to the device address 0x04. The
// IR command codes we are most interested in are 0xFB ("IN-START")
// and 0xFF ("EZ_ADJUST"). Each one opens a different kind of service
// menu on an LG TV.

//#define NEC_IR_ADDRESS_LG_TV 0x04

// The actual remote key command codes we implement here are as
// follows. (Of course, I didn't really _need_ any other than
// the IN_START and the EZ_ADJUST codes, but the others were a
// nice thing to have for initial testing.)
//
//#define IRKEY_CHANNEL_PLUS  0x00
//#define IRKEY_CHANNEL_MINUS 0x01
//#define IRKEY_VOLUME_PLUS   0x02
//#define IRKEY_VOLUME_MINUS  0x03
//#define IRKEY_MUTE          0x09
//#define IRKEY_Q_MENU        0x45
//#define IRKEY_IN_START      0xFB
//#define IRKEY_EZ_ADJUST     0xFF
//


// Since the ATtiny2313 microcontroller only has 2K of flash and
// 128 bytes of SRAM, using a ready-made IR library would
// probably have been out of the question. I just decided to
// implement the NEC protocol on my own, in the simplest possible
// fashion. I found an excellent description of the protocol on
// this page by San Bergmans:
//
// <http://www.sbprojects.com/knowledge/ir/nec.htm>
//
// The carrier frequency for the NEC IR protocol is 38000 Hz. One
// carrier pulse cycle takes 1/38000th of a second. According to
// the page linked above, the recommended carrier duty-cycle for
// this protocol is either 1/4 or 1/3.
//
// Let's figure out what that means in microseconds:
//
// 1/4 cycles takes 1/38000 * 1/4 seconds = 1/152000th of a second.
//
// 1/152000 seconds =  6 11/19 microseconds (=~ 6.58 microseconds)
// 3/152000 seconds = 19 14/19 microseconds (=~ 19.74 microseconds)
// 4/152000 seconds = 26  6/19 microseconds (=~ 26.32 microseconds)
//
// 1/3 cycles takes 1/38000 * 1/3 seconds = 1/114000th of a second.
//
// 1/114000 seconds =  8 44/57 microseconds (=~ 8.77 microseconds)
// 2/114000 seconds = 17 31/57 microseconds (=~ 17.54 microseconds)
// 3/114000 seconds = 26  6/19 microseconds (=~ 26.32 microseconds)
//
// Through some experimenting, I found out the 1/4 duty cycle works
// noticeably better and appears to give a longer range than the 1/3
// duty cycle - at least on my TV and with my LEDs. However, I present
// both timings here in case you want to try them out with your IR
// emitter and TV - maybe they behave differently.

#define DUTY_CYCLE_1_4

#ifdef DUTY_CYCLE_1_4 // 1:4 duty cycle
#define NEC_IR_CARRIER_CYCLE_DURATION_HIGH      (6.58 -4.0)
#define NEC_IR_CARRIER_CYCLE_DURATION_LOW      (19.74 -4.0)
#define NEC_IR_CARRIER_CYCLE_DURATION_TOTAL    26.32
#endif

#ifdef DUTY_CYCLE_1_3 // 1:3 duty cycle
#define NEC_IR_CARRIER_CYCLE_DURATION_HIGH      8.77
#define NEC_IR_CARRIER_CYCLE_DURATION_LOW      17.55 // OK, I just rounded this one up to keep the timebase sane!
#define NEC_IR_CARRIER_CYCLE_DURATION_TOTAL    26.32
#endif

// The following is an enumeration and definition of the four different
// types of mark/space codes (pulse types) found in the NEC IR protocol.
// The first value is an index number, the two other values are the
// "mark" and "space" duration in microseconds.
//
// We want to use these as literal constants so as not to mess up the
// delay loops even on slower processors. Also, the delay.h routines
// cannot be called with variables, only with constants, or they will
// produce inaccurate delays.

#define NEC_IR_LOGICAL_0                          0
#define NEC_IR_LOGICAL_0_MARK_DURATION          560
#define NEC_IR_LOGICAL_0_SPACE_DURATION         560

#define NEC_IR_LOGICAL_1                          1
#define NEC_IR_LOGICAL_1_MARK_DURATION          560
#define NEC_IR_LOGICAL_1_SPACE_DURATION        1690

#define NEC_IR_AGC_CALIBRATION                    2
#define NEC_IR_AGC_CALIBRATION_MARK_DURATION   9000
#define NEC_IR_AGC_CALIBRATION_SPACE_DURATION  4500

#define NEC_IR_END                                3
#define NEC_IR_END_MARK_DURATION                560
#define NEC_IR_END_SPACE_DURATION              4708

// The following function blinks a specific pulse type (as defined
// above) at the 38 kHz carrier. Some nasty hardcoded values in there
// but I hope this helps maintain accuracy with slower CPU speeds. I
// could have used the fancier timer interrupt and pulse width modulator
// features built-in to the microcontroller itself, of course, and
// generate the carrier that way, but this was just a quick busy-loop
// hack for getting into the service menu, anyway... so finer details
// weren't that important. And maybe this kind of a simplistic,
// hardware-independent approach will make it easier to port the same
// code over to other platforms, if required.
/*
void transmit_nec_ir_pulse(uint8_t pulse_type_id) {

	// Calculate how many cycles of the 38 kHz carrier we need
	// for the duration of the "mark" time

	uint16_t num_cycles;

	if( pulse_type_id == NEC_IR_AGC_CALIBRATION )
	num_cycles = 
	(NEC_IR_AGC_CALIBRATION_MARK_DURATION
	/ NEC_IR_CARRIER_CYCLE_DURATION_TOTAL);
	else
	// All other pulse types share an identical 560 us "mark"
	// duration
	num_cycles =
	(NEC_IR_LOGICAL_0_MARK_DURATION
	/ NEC_IR_CARRIER_CYCLE_DURATION_TOTAL);
	
	//AlanFromJapan : ghetto adjustment
	num_cycles = (uint16_t)(CPU_ADJUST * (double)num_cycles);
	
	// Blinking the 38 kHz carrier now!

	for( uint16_t i = 0; i < num_cycles; i++) {
		set_ir_led_on();
		_delay_us(NEC_IR_CARRIER_CYCLE_DURATION_HIGH);
		set_ir_led_off();
		_delay_us(NEC_IR_CARRIER_CYCLE_DURATION_LOW);
	}

	// Wait for the duration of "space" for the given pulse. These
	// delays are hardcoded and split into a couple of separate calls
	// in the hopes they will remain accurate at least in the range
	// of 1...20 MHz CPU speed. See delay.h for details about the
	// allowed (accurate) delay ranges for different Atmel CPU speeds.

	switch( pulse_type_id ) {
		
		case NEC_IR_LOGICAL_0:
		// 560 us
		_delay_ms(0.5 * CPU_ADJUST);
		_delay_us(30 * CPU_ADJUST);
		_delay_us(30 * CPU_ADJUST);
		break;
		case NEC_IR_LOGICAL_1:
		// 1690 us
		_delay_ms(1.6 * CPU_ADJUST);
		_delay_us(30 * CPU_ADJUST);
		_delay_us(30 * CPU_ADJUST);
		_delay_us(30 * CPU_ADJUST);
		break;
		case NEC_IR_END:
		// 4708 us
		_delay_ms(4.7 * CPU_ADJUST);
		_delay_us(8 * CPU_ADJUST);
		break;
		case NEC_IR_AGC_CALIBRATION:
		// 4500 us
		_delay_ms(4.5 * CPU_ADJUST); //_delay_ms(4.5);
		break;
	}
}

void transmit_nec_ir_command(uint8_t address, uint8_t command) {

	uint8_t i, j, value;
	uint8_t bytes[4] = {address, ~address, command, ~command};

transmit_nec_ir_pulse(NEC_IR_AGC_CALIBRATION);

for( i = 0; i < 4; i++ ) {
	
	value = bytes[i];
	
	for( j = 0; j < 8; j++ ) {
		
		if( value & (1<<j) )
		transmit_nec_ir_pulse(NEC_IR_LOGICAL_1);
		else
		transmit_nec_ir_pulse(NEC_IR_LOGICAL_0);
		
	}
}

transmit_nec_ir_pulse(NEC_IR_END);
}


void transmit_nec_ir_commandExt(uint16_t address, uint8_t command) {

	uint8_t i, j, value;
	i = (address >> 8);
uint8_t bytes[4] = { i , address, command, ~command};

transmit_nec_ir_pulse(NEC_IR_AGC_CALIBRATION);

for( i = 0; i < 4; i++ ) {
	
	value = bytes[i];
	
	for( j = 0; j < 8; j++ ) {
		
		if( value & (1<<j) )
		transmit_nec_ir_pulse(NEC_IR_LOGICAL_1);
		else
		transmit_nec_ir_pulse(NEC_IR_LOGICAL_0);
		
	}
}

transmit_nec_ir_pulse(NEC_IR_END);
}
*/
//Theory 560us, real life it's a little shorter because of the surrounding code that causes "inertia"
#define NEC_BIT_MARK 715
//Theory
//#define NEC_ONE_SPACE (2250 - SHARP_BIT_MARK)
//#define NEC_ZERO_SPACE (1125 - SHARP_BIT_MARK)
//Real life (should be 1690 and 560)
#define NEC_ONE_SPACE 1490
#define NEC_ZERO_SPACE 375


//BUFFALO HDD VCR
//Perfect timings as measured with a IR receiver and a logic analyzer
//ON : 705us / 0 LOW 410us / 1 LOW 1540
//HEADER 9180us 4355us

//Send the pulse at 38kHz for pMicroSec duration
void markNEC (double pDurationUS){
	//how many time shall we send the pulse over the period (rounded up)
	uint16_t vCount = 1+ pDurationUS / NEC_IR_CARRIER_CYCLE_DURATION_TOTAL;
	
	for(uint16_t i = 0; i < vCount; i++){
		//on
		PORTD = 0xFF;
		_delay_us(NEC_IR_CARRIER_CYCLE_DURATION_HIGH);
		
		//off
		PORTD = 0x00;
		_delay_us(NEC_IR_CARRIER_CYCLE_DURATION_LOW);
	}
}

inline void spaceNEC (uint16_t pDurationUS){
	//Low
	//off
	PORTD = 0x00;
	_delay_us(pDurationUS);
}

void IRSendNEC(uint16_t address, uint8_t command){
	markNEC(9000);
	
	spaceNEC(4500);
	
	for (uint8_t i = 0; i < 16; i++) {
		if (address & 0x8000) {
			mark(NEC_BIT_MARK);
			space(NEC_ONE_SPACE);
		}
		else {
			mark(NEC_BIT_MARK);
			space(NEC_ZERO_SPACE);
		}
		address <<= 1;
	}
	
	uint8_t commandBar = ~command;
	for (uint8_t i = 0; i < 8; i++) {
		if (command & 0x80) {
			mark(NEC_BIT_MARK);
			space(NEC_ONE_SPACE);
		}
		else {
			mark(NEC_BIT_MARK);
			space(NEC_ZERO_SPACE);
		}
		command <<= 1;
	}	
	for (uint8_t i = 0; i < 8; i++) {
		if (commandBar & 0x80) {
			mark(NEC_BIT_MARK);
			space(NEC_ONE_SPACE);
		}
		else {
			mark(NEC_BIT_MARK);
			space(NEC_ZERO_SPACE);
		}
		commandBar <<= 1;
	}
	
	
	//final marker ?	
	mark(NEC_BIT_MARK);
	
}

#endif /* IR_NEC_H_ */