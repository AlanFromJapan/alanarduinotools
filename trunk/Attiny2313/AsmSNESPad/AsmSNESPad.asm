/*
 * AsmSNESPad.asm
 *
 *  Created: 2012/09/08 23:49:24
 *   Author: Alan
 */ 


.NOLIST
;the includes are here : "C:\Program Files (x86)\Atmel\AVR Studio 5.0\avrassembler\include\"
.INCLUDE "tn2313def.inc"
.LIST

.DEF mCurrentButtonIndex = r2
.DEF mButtonStatusL = r3
.DEF mButtonStatusH = r4

.CSEG
.ORG 0x0000
rjmp main			; first vector : RESET makes go to main
rjmp SIG_INT0		; code for INT0
rjmp SIG_INT1		; code for INT1
rjmp neant 			; Timer1 Capture Handler
rjmp neant 			; Timer1 CompareA Handler
rjmp neant 			; Timer1 Overflow Handler
rjmp neant 			; Timer0 Overflow Handler
rjmp neant 			; USART0 RX Complete Handler
rjmp neant 			; USART0,UDR Empty Handler
rjmp neant 			; USART0 TX Complete Handler
rjmp neant 			; Analog Comparator Handler
rjmp neant 			; Pin Change Interrupt
rjmp neant 			; Timer1 Compare B Handler
rjmp neant 			; Timer0 Compare A Handler
rjmp neant 			; Timer0 Compare B Handler
rjmp neant 			; USI Start Handler
rjmp neant 			; USI Overflow Handler
rjmp neant 			; EEPROM Ready Handler
rjmp neant 			; Watchdog Overflow Handler



; ---------------------------------------------------------
; a dummy handler for unused interrupts
neant:
	nop
	reti

	
; ---------------------------------------------------------
; reads buttoons status
ReadButtonsStatus:
	push r19
	/*
	;TEST DEBUG REMOVE ME TODO
	
	ldi r19, 0xff
	mov mButtonStatusL, r19
	ldi r19, 0x00
	mov mButtonStatusH, r19

	pop r19
	ret
	*/

	;all UP
	ldi r19, 0xff
	mov mButtonStatusL, r19
	mov mButtonStatusH, r19

	sbic	PINB, 0	; 22
	rjmp	ReadButtonsStatus_1      	; 0x44 <__SREG__+0x5>

	ldi	r24, 0xF7	; 247
	mov	mButtonStatusL, r24

ReadButtonsStatus_1:

	sbic	PINB, 1	; 22
	rjmp	ReadButtonsStatus_2      	; 0x4c <__SREG__+0xd>

	ldi	r24, 0x04	; 4
	eor	mButtonStatusL, r24
ReadButtonsStatus_2:

	sbic	PINB, 2	; 22
	rjmp	ReadButtonsStatus_3      	; 0x54 <__SREG__+0x15>

	ldi r24, 0x02
	eor	mButtonStatusH, r24

ReadButtonsStatus_3:
	sbic	PINB, 3	; 22
	rjmp	ReadButtonsStatus_4      	; 0x5c <__SREG__+0x1d>

	ldi	r24, 0x02	; 2
	eor	mButtonStatusL, r24
ReadButtonsStatus_4:

	sbic	PINB, 4	; 22
	rjmp	ReadButtonsStatus_5      	; 0x5c <__SREG__+0x1d>

	ldi	r24, 0x01	; 2
	eor	mButtonStatusH, r24
ReadButtonsStatus_5:

	sbic	PINB, 5	; 22
	rjmp	ReadButtonsStatus_6      	; 0x68 <__SREG__+0x29>

	ldi	r24, 0x01	; 1
	eor	mButtonStatusL, r24
ReadButtonsStatus_6:

	sbic	PINB, 6	; 22
	rjmp	ReadButtonsStatus_7      	; 0x68 <__SREG__+0x29>

	ldi	r24, 0x08
	eor	mButtonStatusH, r24
ReadButtonsStatus_7:

	sbic	PINB, 7	; 22
	rjmp	ReadButtonsStatus_8      	; 0x68 <__SREG__+0x29>

	ldi	r24, 0x04
	eor	mButtonStatusH, r24
ReadButtonsStatus_8:

	////////////////////////////////////////////////////////


	sbic	PIND, 0	; 16
	rjmp	ReadButtonsStatus_9      	; 0x78 <__SREG__+0x39>

	ldi	r24, 0x10	; 16
	eor	mButtonStatusL, r24

ReadButtonsStatus_9:
	sbic	PIND, 1	; 16
	rjmp	ReadButtonsStatus_10      	; 0x80 <__SREG__+0x41>

	ldi	r24, 0x20	; 32
	eor	mButtonStatusL, r24

ReadButtonsStatus_10:
	sbic	PINA, 1	; 25
	rjmp	ReadButtonsStatus_11      	; 0x88 <__SREG__+0x49>

	ldi	r24, 0x80	; 128
	eor	mButtonStatusL, r24

ReadButtonsStatus_11:
	sbic	PINA, 0	; 25
	rjmp	ReadButtonsStatus_12      	; 0x90 <__SREG__+0x51>

	ldi	r24, 0x40	; 64
	eor	mButtonStatusL, r24
ReadButtonsStatus_12:

	;... and return
	pop r19
	ret



; ---------------------------------------------------------
; write next value
writenext:
	push r18
	push r17
	push r16
	;default is HIGH
	;PORTD |= (1 << 4);  //make D4 = 1
	sbi 0x12, 4
;sbi 0x1b, 1	;TEST DEBUG PORTA1
	;if mCurrentButtonIndex >= 8
	MOV r16, mCurrentButtonIndex
	CPI r16, 8
	BRGE writenext_indexSuperior8 
	;case  mCurrentButtonIndex < 8
	MOV r18, mButtonStatusL
	MOV r17, mCurrentButtonIndex
	RJMP writenext_prepareData 
writenext_indexSuperior8: 
	MOV r18, mButtonStatusH
	;r7 = mCurrentButtonIndex -8
	MOV r17, mCurrentButtonIndex
	SUBI r17, 8
writenext_prepareData:
	;now r18 has the value to read, r17 the shift amount, reuse r16 for storing (1 << r17)
	;fill r16
	ldi r16, 1
writenext_prepareData_Loop:
	cpi r17, 0
	breq writenext_dataReady
	;r17--, shift left r16
	lsl r16
	dec r17
	rjmp writenext_prepareData_Loop
writenext_dataReady:
	;now r18 has the value to read, r17 = 0, r16 = (1 << r17)
	AND r18, r16
	CPSE r18, r16 ;r18 has r18 & r16, if equal it means bit was high
	;executed if not equa, means not high, means put it LOW
	;PORTD &= ~(1 << 4); //make D4 = 0
	cbi 0x12, 4
;cbi 0x1b, 1	;TEST DEBUG PORTA1
writenext_outro:
	;go next : mCurrentButtonIndex++;
	inc mCurrentButtonIndex
	;... and back
	pop r16
	pop r17
	pop r18
	ret

	
; ---------------------------------------------------------
; SIG_INT1 interrupt 
; Handles the INT1 pin : LATCH signal going UP (raising hedge) : STARTUP 
SIG_INT1:

	push r16
	;mCurrentButtonIndex = 0
	ldi r16, 0x00
	mov mCurrentButtonIndex, r16

	;send the first value : it will be ready on the next RAISING hedge of INT0 (read)
	rcall writenext

	;and back to the normal process
	pop r16

	reti

	
; ---------------------------------------------------------
; SIG_INT0 interrupt
; Handles the INT0 pin : CLOCK signal going UP (raising hedge) = reading
; trick : since writing on falling for being read on raising is too short, start writing on rising hedge, it will be reading for next reading.
; you can start for the first one by using the latch raising hedge. there you go !
SIG_INT0:

	push r16
	;send the next value
	rcall writenext

	;mCurrentButtonIndex ++
	;done in writenext() inc mCurrentButtonIndex

	;everything was sent ? read status now
	;if mCurrentButtonIndex > 16 call ReadButtonsStatus
	mov r16,mCurrentButtonIndex
	CPI r16, 16
	BRLO SIG_INT0_vect_outro
	rcall ReadButtonsStatus
SIG_INT0_vect_outro:
	;and back to the normal process
	pop r16

	reti
	


; ---------------------------------------------------------
; setup the interrupts
setupInterrupt:
	push r24
	
	//use INT1 and INT0
	//PCMSK |= (1 << PIND3) | (1 << PIND2)
	in	r24, 0x20
	ori	r24, 0x0C
	out	0x20, r24

	// interrupt on INT1 pin falling edge (prepare for reading)
	//INT0 goes on rising hedge (send value on raise, will be read on fall)
	//MCUCR |= (1<<ISC11) | (1<<ISC01) | (1<<ISC11); 
	in	r24, 0x35
	ori	r24, 0x0f ;0x0f = both rising ;0x0A = both falling
	out	0x35, r24

	// turn on interrupts!
	//GIMSK  |= (1<<INT1) | (1<<INT0);
	in	r24, 0x3b
	ori	r24, 0xC0
	out	0x3b, r24

	// go interrupts !
	sei

	pop r24
	ret

; ---------------------------------------------------------
; **                        MAIN                         **
; ---------------------------------------------------------
main:
	//factory settings is to divide internal clock 8MHz by 8.
	//don't, and just run at 8 MHz (set the clock divider to 1 so no effect)
	//CLKPR = (1<<CLKPCE);
	ldi	r24, 0x80
	out	0x26, r24
	//CLKPR = 0; // Divide by 1 
	out	0x26, r1


	//port A is all input with pullups
	//DDRA = 0x00;
	out	0x1a, r1	; 26
	//PORTA = 0xFF;
	ldi	r24, 0xFF	; 255
	out	0x1b, r24	; 27

/*
	//TEST
	//DDRA = 0x00;
	ldi r24, 0xFF
	out	0x1a, r24
	//PORTA = 0xFF;
	ldi	r24, 0x00
	out	0x1b, r24	; 27
*/
	
	//port d as input on all pins (buttons and INT0 and INT1) EXCEPT D4 that is DATA (output)
	//DDRD = 0x10;
	ldi	r25, 0x10	; 16
	out	0x11, r25	; 17
	//pullups everywhere except D2,D3,D4 (INT0, INT1 and DATA)
	//PORTD = 0b11100011;
	ldi	r25, 0xE3	; 227
	out	0x12, r25	; 18
	
	//port b is only inputs (the buttons), with pullups
	//DDRB = 0x00;
	out	0x17, r1	; 23
	//PORTB = 0xFF; //pullups
	out	0x18, r24	; 24
	
	//just make sure pullups are NOT disabled
	//MCUCR |= (0 << PUD); 
	in	r24, 0x35	; 53
	out	0x35, r24	; 53
	
	;all UP
	ldi r19, 0xff
	mov mButtonStatusL, r19
	mov mButtonStatusH, r19

	//turn on the interrupts
	//setupInterrupt();
	rcall setupInterrupt

main_body:
	nop
	rjmp main_body