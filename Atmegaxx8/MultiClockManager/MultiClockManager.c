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
#define USE_DISPLAY_BCD1
#include "MCMLedMatrix.h"
#include "WordclockLayouts.h"
//#include "VoltmeterDisplay.h"

#include "serialComm.h"

/************************************************************************/
/* Setup                                                                */
/************************************************************************/
void mainSetup() {

	//macro to be redefined by each RTC
	RTC_INIT();
	
	//macro to be redefined by each display
	SETUP_DISPLAY(); 
	
	//Start Serial
	serialHardwareInit();
}



/************************************************************************/
/* Main                                                                 */
/************************************************************************/
int main(void) {
	mainSetup();
	
	Date vLastDate;

	USART_SendString("Let's start.\r\n");
	
    while(1) {
/*		
	    //macro to be redefined by each RTC
       RTC_READ_TIME(&vLastDate);
		
		//macro to be redefined by each display
		MAP_DATE_TO_DISPLAY(&vLastDate); 
		
		//macro to be redefined by each display
		DRAW_DISPLAY(); 
*/

	    //macro to be redefined by each RTC
	    RTC_READ_TIME(&vLastDate);

		char vBuff[5];
		itoa(vLastDate.second, vBuff, 8);
		USART_SendString("0x");
		USART_SendString(vBuff);
		USART_SendString("\r\n");
		//small delay
		_delay_ms(1000);
		
		
    }
}