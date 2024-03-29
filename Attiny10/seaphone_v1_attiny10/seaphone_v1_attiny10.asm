/*
 * seaphone_v1_attiny10.asm
 *
 *  Created: 2015/10/28 23:54:20
 *   Author: Alan
 * 
 * For the SeaPhone project, will scan the rotary dial impulse and do something if a given number is entered.
 
                  +-\/-+
 Debug LED    PB0 |o   | PB3  
              GND |    | VCC
 Rotary dial  PB1 |____| PB2  Next (optocoupler)

 */ 

.DEVICE ATtiny10

;----------------------------------------------------------------------
; constants
.EQU F_CPU				= 8000000
.EQU DELAY_PULSE_COUNTA = 255 ; the delay is delaymult2*delaymult1 
.EQU DELAY_PULSE_COUNTB = 3
;255 * 3 = 23970 ~= what we need for 4.2ms delay (doesn't have to be precise, this is a good value per my tests)

.EQU COUNT_DIGIT_PULSE	= 50
; PB0/PB2 are output, PB1 is 0 to be input
.EQU DDRB_BIT_MASK		= ((1 << PB2) | (1 << PB0))
; PB1 is input
.EQU ROTARY_BIT_MASK	= (1 << PB1)
; PB2 is the optocoupler to NEXT button
.EQU NEXT_BIT_MASK	= (1 << PB2)
; PB0 is the debug LED
.EQU LED_BIT_MASK	= (1 << PB0)
;the secret value
.EQU SECRET_VAL			= 1230
;----------------------------------------------------------------------
;variables
.DEF State					= r16
.DEF ReadInProgress			= r17
.DEF Counter				= r18
.DEF LastChangeCounter		= r19
;work register
.DEF w						= r20
.DEF v						= r21
;16 bits word registers
.DEF SecretNumber			= r30 ; and r31

;----------------------------------------------------------------------
.CSEG ; code section
.ORG $0000 ; the starting address
	rjmp main ; for the reset vector
	; no other interrupts should be enabled

/* CAN'T USE, the Tiny10 does not have *mul* operation (yes yes...)
;----------------------------------------------------------------------
;.INCLUDE "avr201.asm": can't use on ATtiny10, there's no r0 or r1... only r16:31

;multiplies Counter16 by w16
mul16x16_16:
	push r16
	push r17

	mul		r28, r26		; al * bl
	movw	r17:r16, r1:r0
	mul		r29, r26		; ah * bl
	add		r17, r0
	mul		r27, r28		; bh * al
	add		r17, r0

	pop r17
	pop r16
	ret
*/



; Use mpy16u from "avr200.asm" (can't include, missing too many registers)
;***************************************************************************
;*
;* "mpy16u" - 16x16 Bit Unsigned Multiplication
;*
;* This subroutine multiplies the two 16-bit register variables 
;* mp16uH:mp16uL and mc16uH:mc16uL.
;* The result is placed in m16u3:m16u2:m16u1:m16u0.
;*  
;* Number of words	:14 + return
;* Number of cycles	:153 + return
;* Low registers used	:None
;* High registers used  :7 (mp16uL,mp16uH,mc16uL/m16u0,mc16uH/m16u1,m16u2,
;*                          m16u3,mcnt16u)	
;*
;***************************************************************************

;***** Subroutine Register Variables

.def	mc16uL	=r16		;multiplicand low byte
.def	mc16uH	=r17		;multiplicand high byte
.def	mp16uL	=r18		;multiplier low byte
.def	mp16uH	=r19		;multiplier high byte
.def	m16u0	=r18		;result byte 0 (LSB)
.def	m16u1	=r19		;result byte 1
.def	m16u2	=r20		;result byte 2
.def	m16u3	=r21		;result byte 3 (MSB)
.def	mcnt16u	=r22		;loop counter

;***** Code

mpy16u:	clr	m16u3		;clear 2 highest bytes of result
	clr	m16u2
	ldi	mcnt16u,16	;init loop counter
	lsr	mp16uH
	ror	mp16uL

m16u_1:	brcc	noad8		;if bit 0 of multiplier set
	add	m16u2,mc16uL	;add multiplicand Low to byte 2 of res
	adc	m16u3,mc16uH	;add multiplicand high to byte 3 of res
noad8:	ror	m16u3		;shift right result byte 3
	ror	m16u2		;rotate right result byte 2
	ror	m16u1		;rotate result byte 1 and multiplier High
	ror	m16u0		;rotate result byte 0 and multiplier Low
	dec	mcnt16u		;decrement loop counter
	brne	m16u_1		;if not done, loop more
	ret


;----------------------------------------------------------------------
;Delay of about DELAY_PULSE_MS
delayPulse:
	push r20
	push r21
	
	ldi r20, DELAY_PULSE_COUNTA
	ldi r21, DELAY_PULSE_COUNTB

	; start delay loop
delayLoop:
	subi r20, 1
	sbci r21, 0
	brne delayLoop
	; end delay loop
	
	pop r21
	pop r20
	ret



;----------------------------------------------------------------------
/************ Main entry point ************/
main:
	; set clock divider : max speed
	ldi r20, 0x00 ; clock divided by 1
	ldi r21, 0xD8 ; the key for CCP
	out CCP, r20 ; Configuration Change Protection, allows protected changes
	out CLKPSR, r21 ; sets the clock divider

	; set pin mask
	ldi w, DDRB_BIT_MASK 
	out DDRB, w ; data direction
	;no pullups
	ldi w, 0x00
	out PUEB, w

	;stop interrupts (not needed in this program)
	cli

	;init variables 
	clr ReadInProgress
	clr State
	clr Counter
	clr LastChangeCounter

	;Read so far is 00000
	ldi SecretNumber, 0
	ldi r31, 0


loop:
	;read PB1
	in w, PINB
	;clean
	andi w, ROTARY_BIT_MASK ;keep PB1 info only

	cp State, w
	breq end_val_diff_state	
val_diff_state:
	mov State, w
	//remember last change time
	clr LastChangeCounter

	cpi State, ROTARY_BIT_MASK 
	brne not_val_is_up
val_is_up:
	//DEBUG LED on
	;sbi PORTB, PB0

	cpi ReadInProgress, 0x00
	brne end_not_read_in_progress
not_read_in_progress:
	ldi ReadInProgress, 0x01
	clr Counter
end_not_read_in_progress:
	inc Counter
	rjmp end_val_is_up
not_val_is_up:
	//DEBUG LED off
	;cbi PORTB, PB0
end_val_is_up:


;No status change, just keep on waiting after a little sleep
end_val_diff_state:
	 



	; if (readingInProgress == 1 && lastChangeMs > 100)
	cpi ReadInProgress, 0x01
	brne end_digit_completed
	cpi LastChangeCounter, COUNT_DIGIT_PULSE
	brlo end_digit_completed
digit_completed:
	clr ReadInProgress
	
	//DEBUG LED on
	;sbi PORTB, PB0



	;read a digit (in Counter), now store

	;Counter16 = Counter16 *10 + Counter
	push r16
	push r17
	push r18
	push r19
	push r20
	push r21
	push r22

	;Put 10 in the multiplicand
	ldi mc16uL,10
	ldi mc16uH,0 
	;Put the existing value in multiplier
	mov mp16uL, SecretNumber ; r30
	mov mp16uH, r31 ; r31
	;Call 16bits x 16bits = 32bits multiply routine
	;NB: use *rcall*, not call!!
	rcall mpy16u 
	;result is in r18-r21 (l->h)
	mov SecretNumber, r18
	mov r31, r19
	;ignore remaining higher 16 bits for now
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r17
	pop r16

	;and now add the counter (so far we have only Counter16 = Counter16 *10)
	;Firstly, 10 becomes 0
	cpi Counter, 10
	brlo counter_less_than_10
	clr Counter
counter_less_than_10:
	;nothing to do

	;Secondly, the addition
	;note the technique: first ADD then ADC. Since we add a single digit, high byte is always zero hence the 'clr w'
	; (see AVR Assembler doc at ADC descr)
	clr w
	add SecretNumber, Counter
	adc r31, w


	; And now check if SecretNumber == SECRET_VAL
	cpi SecretNumber, LOW(SECRET_VAL)
	brne end_digit_completed
	cpi r31, HIGH(SECRET_VAL)
	brne end_digit_completed
	
	;if you're here, SecretNumber == SECRET_VAL
	; Blink blink, you inputed the number!
	
	;Press the <NEXT> key
	sbi PORTB, PB2

	;wait 3ms x 100 = 300ms
	ldi v, 100
next_long_wait:
	cpi v, 0
	breq end_next_long_wait
	dec v
	
	;delay 3ms
	rcall delayPulse
	
	rjmp next_long_wait
end_next_long_wait:

	;clear bit
	cbi PORTB, PB2
		
	;Reset secret number 
	ldi SecretNumber, 0
	ldi r31, 0

end_digit_completed:

	//DEBUG LED off
	;cbi PORTB, PB0


	//DEBUG LED on
	;sbi PORTB, PB0	

/*
// DEBUG blink led if reading number, off if not within a number
	cpi ReadInProgress, 0x01
	brne DBG_ledoff
	//DEBUG LED on
	sbi PORTB, PB0	
	rjmp DBG_end
DBG_ledoff:
	//DEBUG LED off
	cbi PORTB, PB0	  
DBG_end:
	nop
// /DEBUG
*/
	;delay 3ms
	rcall delayPulse

	//DEBUG LED off
	;cbi PORTB, PB0

	ldi w, 1
	add LastChangeCounter, w

	rjmp loop
