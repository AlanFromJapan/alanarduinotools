/*
 * ArithmeticFuncs_tiny10.asm
 *
 *  Created: 2015/11/05 22:58:56
 *   Author: Alan

 Just for the sake of testing ATTiny10 mathematical functions, home made ones too.

 */ 


 
.DEVICE ATtiny10

;----------------------------------------------------------------------
;variables
.DEF State					= r16
.DEF ReadInProgress			= r17
.DEF Counter				= r18
.DEF LastChangePulseCount	= r19
;work register
.DEF w						= r20
;16 bits word registers
.DEF w16					= r26 ; and r27
.DEF Counter16				= r28 ; and r29
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
/************ Main entry point ************/
main:
	; set clock divider : max speed
	ldi r20, 0x00 ; clock divided by 1
	ldi r21, 0xD8 ; the key for CCP
	out CCP, r20 ; Configuration Change Protection, allows protected changes
	out CLKPSR, r21 ; sets the clock divider



loop:
	;Secret num = 0
	clr r30
	clr r31

mul3plus1:
	;Counter16 = Counter16 *10 + Counter
	push r16
	push r17
	push r18
	push r19
	push r20
	push r21
	push r22

	;Put 3 in the multiplicand
	ldi mc16uL,3
	ldi mc16uH,0 
	;Put the existing value in multiplier
	mov mp16uL, SecretNumber ; r30
	mov mp16uH, r31 ; r31
	;Call 16bits x 16bits = 32bits multiply routine
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
	;note the technique: first ADD then ADC. 
	; (see AVR Assembler doc at ADC descr)
	ldi r16, 1
	ldi r17, 0
	add SecretNumber, r16
	adc r31, r17
		
	rjmp mul3plus1