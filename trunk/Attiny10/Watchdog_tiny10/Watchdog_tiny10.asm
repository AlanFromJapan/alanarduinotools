/*
 * Watchdog_tiny10.asm
 *
 *  Created: 2014/11/21 0:13:15
 *   Author: Alan
 */ 

.EQU led_mask = 0xff

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

	ldi r16, 0xff
	out PORTB, r16
	

	;Set the watchdog as interrupt mode
	;cbi WDTCSR, WDE			; clear bit
	;sbi WDTCSR, WDIE		; set watchdog enabled
	;WDP2 | WDP1 => every second
	sei
	ldi r16, 0xD8
	out ccp, r16
	ldi r16, (1 << WDIE | 1 << WDP2 | 1 << WDP1)
	out WDTCSR, r16

	;enable watchdog
	wdr

loop:
	nop
	rjmp loop

WDT_off:
	push r20
	push r21

	in r20, PORTB
	ldi r21, led_mask
	eor r20, r21
	out PORTB, r20

	pop r21
	pop r20

	;reti because it's an interrupt
	reti

