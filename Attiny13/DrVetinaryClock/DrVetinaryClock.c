/*
* DrVetinaryClock.c
*
* Created: 2013/02/16 18:11:19
*  Author: Alan
*/

//#define F_CPU 9600000

#include <avr/io.h>
#include <avr/delay.h>

//maximum number of seconds to skip
#define MAX_RAND 5
//time to sleep between each tick
#define TICK_SLEEP_MS 200
//Approximate duration of 1 step to move 1 tick
#define TICK_DURATION_MS (TICK_SLEEP_MS + 20)
//probability to skip time
#define SKIP_EVERY_N 6

volatile uint8_t vPort = (1 << PB4) | (0 << PB3);

inline void tick(){
	PORTB = vPort;
	_delay_ms(15);
	PORTB = 0x00;

	//reverse status of PB3 and PB4 : the electro-magnet must change direction each time
	vPort ^= (1 << PB4) | (1 << PB3);
	
}

int main(void)
{
	//default uses the 9.6MHz internal crystal ... don't need so much speed
	//CKSEL |= 0x03; //internal oscillator
	
	CLKPR = (0<<CLKPCE); // & (1 << CLKPS3);
	
	//all ports output!
	DDRB = 0xFF;
	
	while (1){
		uint8_t vSkip = rand() % SKIP_EVERY_N;
		
		//if 0, do the time skip stuff, otherwise normal tick
		if (vSkip == 0x00) {
			//How many seconds to sleep
			uint8_t vR = 1 + rand() % MAX_RAND;

			//wait for x additional seconds
			for (uint8_t i = 0; i < vR; i++){
				_delay_ms(1000 - TICK_DURATION_MS);
				
			}
			
			//make the clock move the missing movements
			//One round duration is about MOVE_DURATION_MS
			for (uint8_t i = 0; i < vR; i++){
				tick();
				_delay_ms(TICK_SLEEP_MS);
			}
		}
		else {
			//normal case, just normal tick
			_delay_ms(1000);
			tick();
		}
	}
}
