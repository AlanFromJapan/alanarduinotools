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

	uint8_t i = 0;	
	resetLedMatrix();
	setupLedMatrix();
	
	
	setCell(i,i,1);i++;
	setCell(i,i,1);i++;
	setCell(i,i,1);i++;
	setCell(i,i,1);i++;
	setCell(i,i,1);i++;
	
	//setCell(0, 2, 1);
	//setCell(0, 4, 1);
	//
	setCell(3, 4, 1);

	setCell(4, 3, 1);
	setCell(4, 0, 1);
	
    while(1) {
       mainReadTime(&vLastDate);
		
		//MapTimeInLedMatrix_BCD1(&vLastDate); //MAP_MATRIX_MFUNC(&vLastDate);
		
		drawLedMatrix(); //DRAW_MATRIX_FUNC();
		
		
		//PORTB = 0x03; //=B1 digitalWrite(r + WCLK_ROWA, HIGH);
		//PORTD = 0xe4; //digitalWrite(WCLK_COL1- c, LOW); //minus because reverse order
//
		//PORTD &= ~(1 << 7);
		////POV
		//_delay_us(WCLK_POV_DURATION); // delayMicroseconds(WCLK_POV_DURATION);
//
		////reverse the pins HIGH/LOW status to turn it off
		//PORTB = 0xff; //=B1 digitalWrite(r + WCLK_ROWA, LOW);
		//PORTD = 0xff; //digitalWrite(WCLK_COL1- c, HIGH); //minus because reverse order
		
		
		_delay_ms(50);
    }
}