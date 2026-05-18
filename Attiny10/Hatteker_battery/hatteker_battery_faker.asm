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
.EQU INT_PIN    = 2 ;INT0 pin number (PB2)
.EQU OUT_PIN    = 0 ;output pin number (PB0)

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
    ; rcall (took us here for 3/4 say 4) + push (2) + ldi (1) + pop (2) + ret (4) = 13 => remains 147 for main loop

    push count1

    ldi count1, 36

    ; loop until down to zero 
    ; subi 1 + tst 1 + brne 2 = 4 => 147 // 4 = 36 loops + 3 nops
delay_20us_loop:
    subi count1, 1
    tst count1
    brne delay_20us_loop

    ; remaining 3 cycles  => skip them, my calculation is a little off
    ; nop 
    ; nop
    ; nop

    pop count1
    ret

; ---------------------------------------------------------
; 300 micro second delay routine (assuming 8MHz clock)
delay_300us:
    ; small enough: just call the 20us delay 15 times!
    rcall delay_20us
    rcall delay_20us
    rcall delay_20us
    rcall delay_20us
    rcall delay_20us ; 5
    rcall delay_20us
    rcall delay_20us
    rcall delay_20us
    rcall delay_20us
    rcall delay_20us ; 10
    rcall delay_20us
    rcall delay_20us
    rcall delay_20us
    rcall delay_20us
    rcall delay_20us ; 15
    ret


; ---------------------------------------------------------
; a dummy handler for unused interrupts
neant:
	reti

; ---------------------------------------------------------
; handler for input line change (INT0)
INT0_handler:
    ; no interrupts since we will manipulate the same input line
    ; cli 
    ; not needed as per doc since we don't want nested interrupts (default behavior is fine)
    ; "When an interrupt occurs, the Global Interrupt Enable I-bit is cleared and all interrupts are disabled. The user soft-
    ; ware can write logic one to the I-bit to enable nested interrupts. All enabled interrupts can then interrupt the current
    ; interrupt routine. The I-bit is automatically set when a Return from Interrupt instruction – RETI – is executed."

    ; wait 20us
    rcall delay_20us

    ; toggle the output pin ON 
    sbi PORTB, OUT_PIN  

    ; wait 300us
    rcall delay_300us

    ; toggle the output pin OFF 
    cbi PORTB, OUT_PIN  

    ; give a few us to the pin to stabilize before re-enabling interrupts in case
    rcall delay_20us

    ;resume interrupts!
    ; sei ; not needed as per doc (See above comment about cli)
    
    ; job done
    reti

; ---------------------------------------------------------
; main loop
main: 

	; set clock divider (use in/out, not sbi/cbi since these are memory mapped registers, not I/O ports)
	ldi r16, 0x00 ; clock divided by 1
	ldi r17, 0xD8 ; the key for CCP
	out CCP, r17 ; Configuration Change Protection, allows protected changes
	out CLKPSR, r16 ; sets the clock divider

    ;set PB0 as an output in the Data Direction Register for PORTB
	sbi DDRB, OUT_PIN

    ; all off and no pull ups
	cbi PORTB, OUT_PIN
	cbi PORTB, INT_PIN

    ;Interrupts on INT0 (PB0) on FALLING EDGE
    cbi   EICRA, ISC00  ;set ISC00 to 0
    sbi   EICRA, ISC01  ;set ISC01 to 1 (FALLING EDGE)
    sbi   EIMSK, INT0 ;enable INT0 interrupt

    ;go interrupts!
    sei                 ;enable global interrupts

; main body loop
loop:
    rjmp loop ;infinite loop