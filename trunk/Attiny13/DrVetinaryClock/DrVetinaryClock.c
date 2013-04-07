/*
* DrVetinaryClock.c
*
* Created: 2013/02/16 18:11:19
*  Author: Alan
*/

//4.8Mhz internal with a 8 factor divisor 600kHz : save power and run until lower voltage
#define F_CPU 600000

#include <avr/io.h>
#include <avr/delay.h>

//maximum number of seconds to skip
#define MAX_RAND 4
//time to sleep between each tick
#define TICK_SLEEP_MS 180
//Approximate duration of 1 step to move 1 tick
#define TICK_DURATION_MS (TICK_SLEEP_MS + 20)
//probability to skip time
#define SKIP_EVERY_N 45

//after multiple test, one second shall not be 1,000ms but a little less. This works for me.
#define ONE_SECOND_MS (925)

//duration of the pulse according the voltage
#define TICK_PULSE_MS_5V 7
#define TICK_PULSE_MS_3V 15

volatile uint8_t mPort = (1 << PB4) | (0 << PB3);

inline void tick(){
	PORTB = mPort;
	_delay_ms(TICK_PULSE_MS_3V);
	PORTB = 0x00;

	//reverse status of PB3 and PB4 : the electro-magnet must change direction each time
	mPort ^= (1 << PB4) | (1 << PB3);
	
}

int main(void)
{
	//no diviser : use the 4.8MHz / 8 CLKDIV8 fuse = 600kHz
	CLKPR = (0<<CLKPCE); 
	
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
				_delay_ms(ONE_SECOND_MS - TICK_DURATION_MS);
				
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
			_delay_ms(ONE_SECOND_MS);
			tick();
		}
	}
}
