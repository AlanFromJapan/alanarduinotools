/*
 * ArdReveil4Effects.asm
 *
 *  Created: 2014/11/22 15:36:15
 *   Author: Alan


              +-\/-+
 Flame    PB0 |o   | PB3  
          GND |    | VCC
 Led pin  PB1 |____| PB2  Led pin

 */ 

.DEVICE ATtiny10

; variables
.EQU delayMult1 = 0xff ; the delay is delaymult2*delaymult1 
.EQU delayMult2 = 0x3f

.EQU flame_min = 96
.EQU led_mask = ((1 << PB2) | (1 << PB1))
.EQU pwm_mask = (1 << PB0)

.CSEG
.ORG 0x0000
rjmp main		;1 0x0000 RESET External Pin, Power-on Reset,VLM Reset, Watchdog Reset
rjmp neant		;2 0x0001 INT0 External Interrupt Request 0
rjmp neant		;3 0x0002 PCINT0 Pin Change Interrupt Request 0
rjmp neant		;4 0x0003 TIM0_CAPT Timer/Counter0 Input Capture
rjmp neant		;5 0x0004 TIM0_OVF Timer/Counter0 Overflow
rjmp neant		;6 0x0005 TIM0_COMPA Timer/Counter0 Compare Match A
rjmp neant		;7 0x0006 TIM0_COMPB Timer/Counter0 Compare Match B
rjmp neant		;8 0x0007 ANA_COMP Analog Comparator
rjmp WDT_off	;9 0x0008 WDT Watchdog Time-out
rjmp neant		;10 0x0009 VLM VCC Voltage Level Monitor
rjmp neant		;11 0x000A ADC ADC Conversion Complete (1)



; ---------------------------------------------------------
; a dummy handler for unused interrupts
neant:
	reti

main:	

	; set clock divider
	ldi r16, 0x00 ; clock divided by 1
	ldi r17, 0xD8 ; the key for CCP
	out CCP, r17 ; Configuration Change Protection, allows protected changes
	out CLKPSR, r16 ; sets the clock divider


	; set pin as output
	ldi r16, led_mask ; sets PB2 to output
	out DDRB, r16 ; data direction

	ldi r16, 0x00
	out PORTB, r16
	
	;----------- Watchdog setup start ----------------------------------------
	;README: the fuse WDTON must be UNPROGRAMMED (=not checked)! Otherwise behavior is reset, not interrupt.

	;stop interrupts
	cli

	;no reset on watchdog! (clear bit WDRF in RSTFLR)
	in r16, RSTFLR
	andi r16, ~(1 << WDRF)
	out RSTFLR, r16

	; WDP2 | WDP1 => every second + !WDE | WDIE => interrupt on watchdog timeout
	ldi r17, (1 << WDIE | 1 << WDP2 | 1 << WDP1 | 0 << WDE)
	ldi r16, 0xD8	;0xD8 magic value to write in CCP, and then within 4 cycles you can update WDTCSR
	out CCP, r16
	out WDTCSR, r17

	;start interrupts
	sei
	;----------- Watchdog setup end ----------------------------------------



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
	; this is 8-bit fast pwm with prescaler=1
	ldi r20, (1<<COM0A1)|(1<<WGM00)
	out TCCR0A, r20
	ldi r20, (1<<WGM02)|(1<<CS00)
	out TCCR0B, r20
	
	
	; set pin as output
	in r20, DDRB
	ori r20, pwm_mask ; sets pin 1 (PB0) to putput
	out DDRB, r20 ; data direction
	; set initial value of the pins B1&B2
	ldi r20, 0x02
	out PORTB, r20

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
	;r20 = (r16 & 0x3f) << 1 + flame_min ~= (r16 % 64) * 2 + flame_min
	mov r20, r16
	andi r20, 0x3f
	lsl r20					;r20 *= 2
	ldi r22, flame_min
	add r20, r22

	;r21 = 0x00
	;ldi r21, 0x00			;--> 8bit PWM, not needed

	;set PWM value
	;out OCR0AH, r21		;--> 8bit PWM, not needed
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



; Watchdog interrupt body
WDT_off:
	cli
	push r20
	push r21
	
	;Toggle the pin B1 & B2
	in r20, PORTB
	ldi r21, led_mask
	eor r20, r21
	out PORTB, r20
	
	pop r21
	pop r20
	sei

	;reti because it's an interrupt
	reti

