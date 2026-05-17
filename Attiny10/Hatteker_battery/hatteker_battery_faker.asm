;/*
; * hatteker_battery_faker.asm
; *
; *  Created: 2026/05/17
; *   Author: AlanFromJapan (https://electrogeek.cc/)
; */ 

.include "tn10def.inc"

;----------------------------------------------------------------------
; constants
.EQU F_CPU		= 8000000 ;CPU frequency (8MHz internal oscillator)
.EQU INT_PIN    = 0 ;INT0 pin number (PB0)
.EQU OUT_PIN    = 2 ;output pin number (PB2)

;----------------------------------------------------------------------
;variables

;named registers 
.DEF count1     = r16
.DEF count2     = r17



;----------------------------------------------------------------------
.CSEG ; code section
.ORG $0000 ; the starting address

rjmp main		    ;1 0x0000 RESET External Pin, Power-on Reset,VLM Reset, Watchdog Reset
rjmp INT0_handler	;2 0x0001 INT0 External Interrupt Request 0
rjmp neant		    ;3 0x0002 PCINT0 Pin Change Interrupt Request 0
rjmp neant		    ;4 0x0003 TIM0_CAPT Timer/Counter0 Input Capture
rjmp neant		    ;5 0x0004 TIM0_OVF Timer/Counter0 Overflow
rjmp neant		    ;6 0x0005 TIM0_COMPA Timer/Counter0 Compare Match A
rjmp neant		    ;7 0x0006 TIM0_COMPB Timer/Counter0 Compare Match B
rjmp neant		    ;8 0x0007 ANA_COMP Analog Comparator
rjmp neant		    ;9 0x0008 WDT Watchdog Time-out
rjmp neant		    ;10 0x0009 VLM VCC Voltage Level Monitor
rjmp neant		    ;11 0x000A ADC ADC Conversion Complete (1)


; ---------------------------------------------------------
; 20 micro second delay routine (assuming 8MHz clock)
delay_20us:
    ; 20 microseconds at 8MHz is 160 clock cycles
    ; rjmp (took us here for 2) + push (2) + ldi (1) + pop (2) + ret (4) = 11 => remains 149 for main loop

    push count1

    ldi count1, 49

    ; loop until down to zero 
    ; subi 1 + tst 1 + brne 1 = 3 => 149 // 3 = 49 loops + 2 nops
delay_20us_loop:
    subi count1, 1
    tst count1
    brne delay_20us_loop

    ; remaining 2 cycles
    nop
    nop

    pop count1
    ret

; ---------------------------------------------------------
; 300 micro second delay routine (assuming 8MHz clock)
delay_300us:
    ; small enough: just call the 20us delay 15 times!
    rjmp delay_20us
    rjmp delay_20us
    rjmp delay_20us
    rjmp delay_20us
    rjmp delay_20us ; 5
    rjmp delay_20us
    rjmp delay_20us
    rjmp delay_20us
    rjmp delay_20us
    rjmp delay_20us ; 10
    rjmp delay_20us
    rjmp delay_20us
    rjmp delay_20us
    rjmp delay_20us
    rjmp delay_20us ; 15
    ret


; ---------------------------------------------------------
; a dummy handler for unused interrupts
neant:
	reti

; ---------------------------------------------------------
; handler for input line change (INT0)
INT0_handler:
    ; no interrupts since we will manipulate the same input line
    cli

    ; wait 20us
    rjmp delay_20us

    ; toggle the output pin ON (PB2)
    sbi   PORTB, OUT_PIN  
    ; wait 300us
    rjmp delay_300us
    ; toggle the output pin OFF (PB2)
    cbi   PORTB, OUT_PIN  

    ; give a few us to the pin to stabilize before re-enabling interrupts in case
    rjmp delay_20us

    ;resume interrupts!
    sei
    
    ; job done
    reti

; ---------------------------------------------------------
; main loop
main: 
    ;set PB2 as an output in the Data Direction Register for PORTB
    sbi   DDRB, OUT_PIN  ;connect LED to PB2 (Attiny10 pin 4)

    ;Interrupts on INT0 (PB0) on FALLING EDGE
    cbi   EICRA, ISC00  ;set ISC00 to 0
    sbi   EICRA, ISC01  ;set ISC01 to 1 (FALLING EDGE)
    sbi   EIMSK, INT_PIN ;enable INT0 interrupt

    ;go interrupts!
    sei                 ;enable global interrupts

; main body loop
loop:
    rjmp loop ;infinite loop