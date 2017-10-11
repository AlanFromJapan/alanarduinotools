/*
 * RaspiPower_tiny10.asm
 *
 *  Created: 2017/10/05 23:17:18
 *   Author: Alan


                   +-\/-+
 <DoShutdown>  PB0 |o   | PB3  >Pi is on< /RESET
               GND |    | VCC
 >Button<      PB1 |____| PB2  <Relay>

 */ 


 .DEVICE ATtiny10

;----------------------------------------------------------------------
; constants
.EQU F_CPU				= 8000000

;delay for do shutdown, should be a handful of MS
.EQU DELAY_PULSE_COUNTA = 10 ; the delay is delaymult2*delaymult1 
.EQU DELAY_PULSE_COUNTB = 1

; PB0/PB2 are output, PB1 is input
.EQU DDRB_BIT_MASK		= ((1 << PB2) | (1 << PB0))
;button filter
.EQU BUTTON_MASK		= 0x02

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
; ****************** Wait for button pressed ****************** 
wait_for_button:
	;read PB1
	in w, PINB	
	;clean
	andi w, BUTTON_MASK ;keep PB1 info only

	;button has pullup
	cpi w, BUTTON_MASK
	breq wait_for_button	
wait_for_button_is_pressed:
	ret

;----------------------------------------------------------------------
; ****************** Set relay ON ****************** 
set_relay_on:
	sbi PORTB, PB2
	ret

;----------------------------------------------------------------------
; ****************** Set relay OFF ****************** 
set_relay_off:
	cbi PORTB, PB2
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
; ****************** Send DO SHUTDOWN ****************** 
send_do_shutdown:
	;PB0 goes HIGH
	sbi PORTB, PB0

	;let it HIGH a bit
	rcall delayPulse

	;PB0 goes LOW
	cbi PORTB, PB0

	ret


;----------------------------------------------------------------------
; ****************** wait a little ****************** 
wait_small:
	push w
	clr w
loop_delay_off3:
	inc w

	rcall delayPulse

	cpi w, 5
	brne loop_delay_off3
	pop w
	ret

;----------------------------------------------------------------------
; ****************** wait longer ****************** 
wait_long:
	push w
	clr w
loop_delay_off2:
	inc w

	rcall delayPulse

	cpi w, 50
	brne loop_delay_off2
	pop w
	ret

;----------------------------------------------------------------------
; ****************** Main entry point ****************** 
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

	;////////////////////////// TEST //////////////////////////////
	;just wait 30 sec-ish

	//rjmp delayPulse

	;button pressed > Turn on the relay
	rcall set_relay_on

	;just wait 5 sec-ish
	rcall wait_small

	;Turn OFF the relay
	rcall set_relay_off
	;////////////////////////// /TEST //////////////////////////////

loop:
	;wait for first press to turn on
	rcall wait_for_button

	;button pressed > Turn on the relay
	rcall set_relay_on

	;just wait 5 sec-ish
	rcall wait_small
		
	;wait for next press to turn off
	rcall wait_for_button

	;send the shudown signal
	rcall send_do_shutdown

	;version simple: no reading of the /RESET - Pi_is_on pin
	;just wait 30 sec-ish
	rcall wait_long

	;Turn OFF the relay
	rcall set_relay_off


	;just wait 5 sec-ish
	rcall wait_small

	;Loop!
	rjmp loop
	 
