/*
 * Blink_tiny10.asm
 *
 *  Created: 2014/11/16 22:49:10
 *   Author: Alan
 */ 

  rjmp RESET  ;go and set up PORTB as an output 


;name registers (selected >r15 arbitrarily)
 .def  counter1  = r16
 .def  counter2  = r17
 .def  counter3  = r18 


;set some variables 
 ;time1 and time2 set the value for the final loop in each delay
 .equ  time1   = 170 ;between 0 and 255
 .equ  time2   = 10
 .equ  led     = 2 ;LED at PB2 
   
RESET: ;set PB2 as an output in the Data Direction Register for PORTB
 sbi   DDRB, led  ;connect LED to PB2 (Attiny10 pin 4)
      
flash: ;main loop  


 cbi   PORTB, led ;LED off - cbi/sbi swapped for N-FET switching (ie.LED is OFF when FET is ON)
 ldi   counter2, time1 ;load counter1 delay         
 rcall onDelay            
 sbi   PORTB, led ;LED on           
 ldi   counter3, time2 ;load counter3 delay
 rcall offDelay
 rjmp  flash  ;return to beginning of loop


onDelay:       
 clr   counter1  ;clear counter1 


loop1: ;nested loop that decrements counter 1 (255) x counter2 (time1) times (ie. 255*time1)
 dec   counter1  ;decrement counter1 
 brne  loop1     ;branch if not 0     
 dec   counter2  ;decrement counter2 
 brne  loop1     ;branch if not 0     
 ret      
                
offDelay: ;same as onDelay but with a third loop     
 clr   counter1
 clr   counter2 


loop2: ;decrement counter 1(255) x counter2(255) x counter3(time2) (ie. 255*255*time2) 
 dec   counter1
 brne  loop2         
 dec   counter2
 brne  loop2          
 dec   counter3 
 brne  loop2 
 ret   