/*
 * MultiClockManager.c
 *
 * At last, I start the portage to AtmelStudio of the WordClock project made under Arduino.
 * This program is made to serve all the clocks I produced, disregarding the RTC or display used.
 * http://kalshagar.wikispaces.com/WordClock
 *
 * Created: 2014/11/23 11:23:47
 *  Author: Alan
 *
 * Migration Status:
 *   RTC:
 *     O Fake
 *     x DS3231
 *     O DS3234
 *     x EPSON RTC 4543 SA/SB
 *   Display:
 *     x Numitron
 *     O Voltmeter
 *     O BCD v1 (5x5 led matrix)
 * 
 */ 

//for delay.h >> defined in the compilation symbols
//#define F_CPU 16000000L

#include <stdlib.h>

//System includes
#include <avr/io.h>
#include <util/delay.h>

//Program includes
#include "MCMShared.h"

//Choose what RTC to use
#include "DS3231.h"
//#include "DS3234.h"
//#include "RTCFake.h"


//Choose the display to use (put the define BEFORE the includes!)
//#define USE_DISPLAY_BCD1
//#include "MCMLedMatrix.h"
//#include "WordclockLayouts.h"
//#include "VoltmeterDisplay.h"

#include "serialComm.h"

/************************************************************************/
/* Setup                                                                */
/************************************************************************/
void mainSetup() {

	//Start Serial
	serialHardwareInit();
	USART_SendString("\n\n------------------------");

	//macro to be redefined by each RTC
	RTC_INIT();
	
	//Set time! (don't do it every time ;) Remove me later)
	//SetTimeDate3231(NULL);

	//macro to be redefined by each display
	//SETUP_DISPLAY();
	
}



/************************************************************************/
/* Main                                                                 */
/************************************************************************/
int main(void) {
	mainSetup();

	//C0 as out
	DDRC |= 0x01;
	//C0 off
	PORTC &= ~0x01;
	
	Date vLastDate;

	USART_SendString("\nLet's start.\n");
	
    while(1) {

	    //macro to be redefined by each RTC
    	RTC_READ_TIME(&vLastDate);
		
		//macro to be redefined by each display
		//MAP_DATE_TO_DISPLAY(&vLastDate);
		
		//macro to be redefined by each display
		//DRAW_DISPLAY();


	    //macro to be redefined by each RTC
//	    RTC_READ_TIME(&vLastDate);

    	USART_SendString("h=");
		char vBuff[15];
		itoa(vLastDate.second, vBuff, 10);
		USART_SendString(vBuff);

		USART_SendString(". ");


    	//C0 off
    	PORTC &= ~0x01;

	    //small delay
		_delay_ms(500);

		//C0 on
		PORTC |= 0x01;

	    //small delay
		_delay_ms(500);

		
    }
}
