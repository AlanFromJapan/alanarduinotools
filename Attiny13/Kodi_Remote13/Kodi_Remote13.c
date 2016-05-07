/*
 * Kodi_Remote13.c
 *
 * Created: 2016/05/07 18:04:16
 *  Author: Alan
 */ 


//MUST DEFINE F_CPU in the PROJECT SYMBOLS!!!
//#define F_CPU			4800000

#include <avr/io.h>
#include <avr/delay.h>

#include "Serial13.h"

int main(void)
{
	//setups
	setupSerial();
	setupADC();

		
    while(1)
    {
		uint8_t v = doADC();
		if (v == 0xc4){
			serialSendString("clicked()\n");
			_delay_ms(1500);
		}		
		_delay_ms(50);
    }
}