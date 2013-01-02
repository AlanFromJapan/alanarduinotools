/*
 * Test2313.c
 *
 * Created: 2012/08/01 22:47:03
 *  Author: Alan
 */ 

#define F_CPU 1000000UL	      // Sets up the default speed for delay.h

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	//TestPullups();
	
	AquestLSI_Init();
	_delay_ms(1000);
	
    while(1)
    {
		//SayKonnichiwa();
	    SayAll();
    }
}