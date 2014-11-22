/*
 * FakeFlame_tiny10.asm
 *
 *  Created: 2014/11/18 22:35:08
 *   Author: Alan
 */ 
 
.DEVICE ATtiny10

; variables
.EQU delayMult1 = 0xff ; the delay is delaymult2*delaymult1 
.EQU delayMult2 = 0x3f

.EQU meanVal = 127
.EQU radius = 50

.CSEG ; code section
.ORG $0000 ; the starting address
	rjmp main ; for the reset vector
	; no other interrupts should be enabled

main:
	; set clock divider
	ldi r20, 0x00 ; clock divided by 1
	ldi r21, 0xD8 ; the key for CCP
	out CCP, r21 ; Configuration Change Protection, allows protected changes
	out CLKPSR, r20 ; sets the clock divider
	
	; set up timer and compare values
	; TCCR0A contains [COM0A1, COM0A0, COM0B1, COM0B0,-,-, WGM01, WGM00]
	; means: compare output mode(what pins do), wave mode
	;
	; TCCR0B contains [ICNC0, ICES0, -, WGM03, WGM02, CS02, CS01, CS00]
	; means: input capture noise cancel and edge select, wave mode, clock select
	;
	; TCCR0C contains [FOC0A, FOC0B, -,-,-,-,-,-]
	; means: force compare
	;
	; TIMSK0 contains [-,-, ICIE0,-,-, ICIE0B, ICIE0A, TOIE0]
	; means: interrupt enable
	;
	; TIFR0 contains [-,-, ICF0,-,-, OCF0B, OCF0A, TOV0]
	; means: interrupt flags
	;
	; GTCCR contains [TSM,-,-,-,-,-,-, PSR]
	; means: holds prescaler reset(halts count) , prescaler reset
	;
	; TCNT0 H:L is counter register
	; OCR0A H:L is compare register A
	; OCR0B H:L is compare register B
	; ICR0 H:L is input capture register
	;
	
	; set up PWM type and clock
	; this is 10-bit fast pwm with prescaler=1
	ldi r20, (1<<COM0A1)|(1<<WGM01)|(1<<WGM00)
	out TCCR0A, r20
	ldi r20, (1<<WGM02)|(1<<CS00)
	out TCCR0B, r20
	
	
	; set pin as output
	ldi r20, 1<<PB0 ; sets pin 1 (PB0) to putput
	out DDRB, r20 ; data direction
	
	; nop for sync
	nop
	
init_vars_rnd:
	; init the value so it can be randomized
	ldi r16, 0xad
	ldi r17, 0xad
	ldi r18, 0xad
	ldi r19, 0xad


loop:
	; delay
	rcall delay

	;new rand
	rcall rand_31

	;use just r17 and r16, and center around 
	;r20 = (r16 & 0x3f) + meanVal ~= (r16 % 64) + meanVal
	mov r20, r16
	andi r20, 0x3f
	ldi r22, meanVal
	add r20, r22

	;r21 = 0x00
	ldi r21, 0x00

	;set PWM value
	out OCR0AH, r21
	out OCR0AL, r20
	
	rjmp loop

delay:
	push r20
	push r21
	
	ldi r20, delayMult1
	ldi r21, delayMult2

	; start delay loop
delayLoop:
	subi r20, 1
	sbci r21, 0
	brne delayLoop
	; end delay loop
	
	pop r21
	pop r20
	ret

; http://www.avrfreaks.net/forum/very-fastsmall-random-number-generator
; 31-bit random number
; primitive polynomial order 31
; (1)0100000 10101010 10010101 10110101 (20 AA 95 B5)
; shift before XOR
rand_31:
	push r24

	lsl	r16			; shift first
	rol	r17
	rol	r18
	rol	r19

	sbrs	r19,7			; test MSB
	rjmp	rand_31_outro			; clear, no XOR

	ldi	r24,0xB5		; first poly byte
	eor	r16,r24			; XOR it in
	ldi	r24,0x95		; second byte of poly
	eor	r17,r24
	ldi	r24,0xAA		; same routine
	eor	r18,r24
	ldi	r24,0x20
	eor	r19,r24

rand_31_outro:
	pop r24
	ret