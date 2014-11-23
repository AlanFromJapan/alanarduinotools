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
 *     x DS3231
 *     O DS3234
 *     x EPSON RTC 4543 SA/SB
 *   Display:
 *     x Numitron
 *     x Voltmeter
 *     O BCD v1 (5x5 led matrix)
 * 
 */ 

//for delay.h
#define F_CPU 16000000L

//System includes
#include <avr/io.h>

//Program includes
#include "MCMShared.h"

//Choose what RTC to use
#include "DS3234.h"
//#include "RTCFake.h"

//Choose the display to use (put the define BEFORE the includes!)
#define USE_DISPLAY_BCD1
#include "MCMLedMatrix.h"
#include "WordclockLayouts.h"


/************************************************************************/
/* Read time functions                                                  */
/************************************************************************/
void mainReadTime(Date* pTime){
	
	#ifdef RTC_DS3231
	getDateDS3231(pTime);
	#endif //RTC_DS3231

	#ifdef RTC_DS3234
	ReadTime3234(pTime);
	#endif //RTC_DS3234

	#ifdef RTC_RTC4543
	ReadTime4543(pTime);
	#endif //RTC_RTC4543
	
	#ifdef RTC_FAKE
	ReadTimeFake(pTime);
	#endif //RTC_FAKE
}

/************************************************************************/
/* Setup                                                                */
/************************************************************************/
void mainSetup() {

#ifdef RTC_DS3234
	setupDS3234(1);
	
	//README TODO REMOVE ME : just for the testing purpose now
	//SetTimeDate3234(25,05,2013,18,14,00);
#endif //RTC_DS3234	

	//macro to be redefined by each display
	SETUP_DISPLAY(); 
}



/************************************************************************/
/* Main                                                                 */
/************************************************************************/
int main(void) {
	mainSetup();
	
	Date vLastDate;

    while(1) {
       mainReadTime(&vLastDate);
		
		//macro to be redefined by each display
		MAP_DATE_TO_DISPLAY(&vLastDate); 
		
		//macro to be redefined by each display
		DRAW_DISPLAY(); 
			
		//small delay
		_delay_ms(5);
    }
}