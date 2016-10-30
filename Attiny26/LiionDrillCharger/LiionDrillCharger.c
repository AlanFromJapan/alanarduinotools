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

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRA = 0xff;
	
	PORTA = 0xff;
    while(1)
    {
		_delay_ms(500);
		PORTA = ~PORTA;
    }
}