/*
 * seaphone_v1_attiny10.asm
 *
 *  Created: 2015/10/28 23:54:20
 *   Author: Alan
 * 
 * For the SeaPhone project, will scan the rotary dial impulse and do something if a given number is entered.
 */ 

.DEVICE ATtiny10

; constants
.EQU DELAY_PULSE_MS = 3
.EQU DELAY_DIGIT_MS = 100
.EQU COUNT_DIGIT_PULSE = DELAY_DIGIT_MS / DELAY_PULSE_MS
; PB2 is output, PB1 is 0 to be input
.EQU BIT_MASK = ((1 << PB2))

;variables
.DEF State					= r16
.DEF ReadInProgress			= r17
.DEF Counter				= r18
.DEF LastChangePulseCount	= r19
;work register
.DEF w						= r20

.CSEG ; code section
.ORG $0000 ; the starting address
	rjmp main ; for the reset vector
	; no other interrupts should be enabled


/************ Main entry point ************/
main:
	; set clock divider : max speed
	ldi r20, 0x00 ; clock divided by 1
	ldi r21, 0xD8 ; the key for CCP
	out CCP, r20 ; Configuration Change Protection, allows protected changes
	out CLKPSR, r21 ; sets the clock divider

	; set pin mask
	ldi w, BIT_MASK 
	out DDRB, w ; data direction
	;no pullups
	ldi w, 0x00
	out PORTB, w

	;stop interrupts (not needed in this program)
	cli


loop:
	nop 
	rjmp loop
