/*
 * RaspiPower_tiny10.asm
 *
 *  Created: 2017/10/05 23:17:18
 *   Author: Alan


                   +-\/-+
 <DoShutdown   PB0 |o   | PB3  
               GND |    | VCC
 >Button       PB1 |____| PB2  <Relay

 */ 


 .DEVICE ATtiny10

;----------------------------------------------------------------------
; constants
.EQU F_CPU				= 8000000
; PB0/PB2 are output, PB1 is input
.EQU DDRB_BIT_MASK		= ((1 << PB2) | (1 << PB0))

;----------------------------------------------------------------------
;variables

;work register
.DEF w						= r20
.DEF v						= r21

;----------------------------------------------------------------------
.CSEG ; code section
.ORG $0000 ; the starting address
	rjmp main ; for the reset vector
	; no other interrupts should be enabled


;----------------------------------------------------------------------
/************ Main entry point ************/
main:
	; set clock divider : max speed
	ldi r20, 0x08 ; clock divided by 256
	ldi r21, 0xD8 ; the key for CCP
	out CCP, r20 ; Configuration Change Protection, allows protected changes
	out CLKPSR, r21 ; sets the clock divider

	; set pin mask
	ldi w, DDRB_BIT_MASK 
	out DDRB, w ; data direction
	;pullup on B1
	ldi w, (1<<PUEB1)
	out PUEB, w	


loop_intro:
	clr w
	clr v

	

loop:
	inc w

	cpi w, 0
	brne loop
	
	inc v

	cpi v, 100
	brne loop

	;now 256 times w x 100 times v have passed

	;Turn on the Relay
	in w, PORTB
	andi w, 0x04
	cpi w, 0x04

	breq loop_relay_clear

loop_relay_set:

	sbi PORTB, PB2
	rjmp loop_outro

loop_relay_clear:
	cbi PORTB, PB2

loop_outro:
	clr w
	clr v

	;Loop!
	rjmp loop
	 
