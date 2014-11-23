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
 *     x DS3234
 *     x EPSON RTC 4543 SA/SB
 *   Display:
 *     x Numitron
 *     x Voltmeter
 * 
 */ 

//for delay.h
#define F_CPU 16000000L

//System includes
#include <avr/io.h>

//Program includes
#include "MCMShared.h"

//Choose what RTC to use
//#include "DS3234.h"
#include "RTCFake.h"

//Choose the display to use
#include "MCMLedMatrix.h"
#include "WordclockLayouts.h"
#define USE_DISPLAY_BCD1


/************************************************************************/
/* Read time functions                                                  */
/************************************************************************/
void mainReadTime(Date* pTime){
	
	#ifdef RTC_DS3231
	getDateDS3231(pTime);
	#endif //RTC_DS3231

	#ifdef RTC_DS3234
	ReadTime(pTime);
	#endif //RTC_DS3234

	#ifdef RTC_RTC4543
	ReadTime4543(pTime);
	#endif //RTC_RTC4543
	
	#ifdef RTC_FAKE
	ReadTime(pTime);
	#endif //RTC_FAKE
}

/************************************************************************/
/* Setup                                                                */
/************************************************************************/
void setup() {

#ifdef RTC_DS3234
	setupDS3234(1);
	
	//README TODO REMOVE ME : just for the testing purpose now
	SetTimeDate(25,05,2013,18,14,00);
#endif //RTC_DS3234	

//TODO: surround with #ifdef
setupLedMatrix(); //SETUP_MATRIX();
}



/************************************************************************/
/* Main                                                                 */
/************************************************************************/
int main(void) {
	setup();
	
	Date vLastDate;

	resetLedMatrix();
	setupLedMatrix();

	
    while(1) {
       mainReadTime(&vLastDate);
		
		MapTimeInLedMatrix_BCD1(&vLastDate); //MAP_MATRIX_MFUNC(&vLastDate);
		
		drawLedMatrix(); //DRAW_MATRIX_FUNC();
			
		
		_delay_ms(5);
    }
}