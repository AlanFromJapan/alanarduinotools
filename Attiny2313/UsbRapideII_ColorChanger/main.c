/*
 * main.c
 *
 *  Created on: Oct 17, 2023
 *      Author: alan
 */



//defined in the compilation params "-D F_CPU=12000000" or define it here
//#define F_CPU 12000000

#include <avr/io.h>
#include <avr/iotn2313.h> //because couldn't find TCCR0B register in usbHardwareInit()
#include <util/delay.h>     // for _delay_ms()


#define LED2_REG	DDRB
#define LED2_PORT	PORTB
#define LED2_R		0x02
#define LED2_G		0x08
#define LED2_B		0x04
#define LED2_MASK	(LED2_R | LED2_G | LED2_B)



static  void ledsOn(){
	LED2_PORT |= LED2_MASK;
}

static  void ledsOff(){
	LED2_PORT &= ~(LED2_MASK);
}

static  void red2(uint8_t pVal){
	if (pVal == 0){
		//OFF
		LED2_PORT &= ~(LED2_R);
	}
	else {
		//ON
		LED2_PORT |= LED2_R;
	}
}

static void green2(uint8_t pVal){
	if (pVal == 0){
		//OFF
		LED2_PORT &= ~(LED2_G);
	}
	else {
		//ON
		LED2_PORT |= LED2_G;
	}
}

static void blue2(uint8_t pVal){
	if (pVal == 0){
		//OFF
		LED2_PORT &= ~(LED2_B);
	}
	else {
		//ON
		LED2_PORT |= LED2_B;
	}
}



/************************************************************************/
/* MAIN                                                                 */
/************************************************************************/
int main(void) {


	//2: 3 tact switch setup (pullups)
	//PB5 input
	DDRB &= ~0x20;
	//Pull up on PB5
	PORTB |= 0x20;

	//----------------------------------
	//3: the RGB led
	LED2_REG |= LED2_MASK;
	ledsOff();

	//just make sure pullups are NOT disabled
	MCUCR |= (0 << PUD);

	double t = 600 / 6;

	//MAIN LOOP
	for(;;){


			green2(1);
			_delay_ms(t);

			green2(0);
			_delay_ms(t);


			blue2(1);
			_delay_ms(t);

			blue2(0);
			_delay_ms(t);



			red2(1);
			_delay_ms(t);

			red2(0);
			_delay_ms(t);

	}

	return 0;
}
