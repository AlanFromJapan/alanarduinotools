;
; PWM
; uses PWM to fade an LED on pin 1 (PB0)
; the fade follows a (sin(t)+1)/2 pattern
; that is stored in a table in memory
; 

.DEVICE ATtiny10

; variables
.EQU delayMult1 = 0xff ; the delay is delaymult2*delaymult1 
.EQU delayMult2 = 0x3f

.DEF sinhalf = r22 ; holds the current half of the wave
.DEF currentindex = r21 ; holds the current index in the table

.CSEG ; code section
.ORG $0000 ; the starting address
	rjmp main ; for the reset vector
	; no other interrupts should be enabled




main:
	; set up the stack
	ldi r16, high(RAMEND)
	out SPH, r16
	ldi r16, low(RAMEND)
	out SPL, r16
	
	; set clock divider
	ldi r16, 0x00 ; clock divided by 1
	ldi r17, 0xD8 ; the key for CCP
	out CCP, r17 ; Configuration Change Protection, allows protected changes
	out CLKPSR, r16 ; sets the clock divider
	
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
	ldi r16, (1<<COM0A1)|(1<<WGM01)|(1<<WGM00)
	out TCCR0A, r16
	ldi r16, (1<<WGM02)|(1<<CS00)
	out TCCR0B, r16
	
	
	; set the pwm duty cycle
	out OCR0AH, r25
	out OCR0AL, r24
	
	; set pin as output
	ldi r16, 1<<PB0 ; sets pin 1 (PB0) to putput
	out DDRB, r16 ; data direction
	
	; nop for sync
	nop

loop:
	; delay
	rcall delay
	
	; change the pwm duty cycle
	inc r24

	ldi r16, 255
	cp r24,r16
	breq plusone
	out OCR0AH, r25
	out OCR0AL, r24
	
	nop
	rjmp loop

delay:
	push r16
	push r17
	
	ldi r16, delayMult1
	ldi r17, delayMult2

	; start delay loop
delayLoop:
	subi r16, 1
	sbci r17, 0
	brne delayLoop
	; end delay loop
	
	pop r17
	pop r16
	ret

plusone:
	inc r25
	ldi r24, 0

	;1024 => r25 = 0x04, r24 = 0x00

	ldi r16, 4
	cp r25, r16
	breq reset_cnt
	ret
reset_cnt:
	ldi r24, 0
	ldi r25, 0
	ret