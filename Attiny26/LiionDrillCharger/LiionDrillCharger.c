/*
 * LiionDrillCharger.c
 *
 * Created: 2016/10/30 11:00:53
 *  Author: Alan
 *
 * Project page: http://electrogeek.cc/drill%20battery%20from%20laptop%20battery.html
 * Micro controller runs at 1MHz internal oscillator (factory settings)
 *
 */ 

//CPU is 1MHz
#define F_CPU 1000000

#define BATTERY_COUNT	4
#define LED1_PINB		6
#define RELAY1_PINA		4

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	//B is all out
	DDRB = 0xff;
	//A is in for A0-1, out for the rest
	DDRA = 0xfc;
	
	PORTB = 0x00;
	PORTA = 0x00;
	
	uint8_t vBattery = 0;
    while(1)
    {
		PORTB = 1 << (LED1_PINB - vBattery);
		PORTA = 1 << (RELAY1_PINA + vBattery);
		
		_delay_ms(1000);
		
		vBattery++;
		if (vBattery >= BATTERY_COUNT){
			vBattery = 0;
		}		
    }
}