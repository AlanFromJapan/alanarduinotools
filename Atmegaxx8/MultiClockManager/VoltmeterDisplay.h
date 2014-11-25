/*
 * VoltmeterDisplay.h
 *
 * Created: 2014/11/25 19:45:35
 *  Author: Alan
 */ 


#ifndef VOLTMETERDISPLAY_H_
#define VOLTMETERDISPLAY_H_

#include "MCMShared.h"
#include <avr/eeprom.h>

#define USE_DISPLAY_VOLTMETER

#ifdef  USE_DISPLAY_VOLTMETER
	#define MAP_DATE_TO_DISPLAY(p)	voltmeterMapDate(p);
	#define DRAW_DISPLAY()			voltmeterDrawDisplay();
	#define SETUP_DISPLAY()			voltmeterSetup() ;

	#define SET_EDIT_HOURS() 		;
	#define SET_EDIT_MINUTES() 		;
	#define SET_EDIT_FINISH() 		;	
#endif


//Contains the calibration for the hours
#define EE_HOURS_COUNT 12
uint8_t EEMEM EE_HOURS_CALIBRATION[EE_HOURS_COUNT];

//Store the latest date received
Date mLatestDate;

#define VOLTMETER_PWM_PIN_HOURS 0x40 /*= pin D6 */

/************************************************************************/
/* Setup for the PWM outputs                                            */
/************************************************************************/
void voltmeterSetupPWM(){
	
	DDRD |= 0xff;// VOLTMETER_PWM_PIN_HOURS;
	// PD6 is now an output

	OCR0A = 0;
	// set PWM for 0% duty cycle


	TCCR0A |= (1 << COM0A1);
	// set none-inverting mode

	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	// set fast PWM Mode

	TCCR0B |= (1 << CS01);
	// set prescaler to 8 and starts PWM


}
	
/************************************************************************/
/* stores values in memory once                                         */
/************************************************************************/
void voltmeterSetup() {	
	uint8_t vHours[EE_HOURS_COUNT];
	
	voltmeterSetupPWM();
	
	eeprom_read_block((void*)vHours, (const void*)EE_HOURS_CALIBRATION, EE_HOURS_COUNT);
	
	//will program ONCE if last value = 0, otherwise leave what is there (unprogrammed eeprom block read 0xff)
	if (0xff == vHours[0]){
		//my results with a 1kOhm resistor and my hours voltmeter, barely unused
		uint8_t i = 0;
		vHours[i++] = 0;
		vHours[i++] = 15;
		vHours[i++] = 40;
		vHours[i++] = 55;
		vHours[i++] = 70;
		vHours[i++] = 85;
		vHours[i++] = 100;
		vHours[i++] = 115;
		vHours[i++] = 140;
		vHours[i++] = 165;
		vHours[i++] = 195;
		vHours[i++] = 240;
		
		eeprom_update_block((const void*)vHours, (void*)EE_HOURS_CALIBRATION, EE_HOURS_COUNT);
	}
}

/************************************************************************/
/* Store parameter as latest date                                       */
/************************************************************************/
void voltmeterMapDate(Date* pDate){
	//memcpy(&mLatestDate, (const void*)pDate, size_t(Date));
	
	//whatever, just need hours and minutes
	mLatestDate.hour = pDate->hour;
	mLatestDate.minute = pDate->minute;
}

/************************************************************************/
/* Draws the latest received date/time to the voltmeters                */
/************************************************************************/
void voltmeterDrawDisplay(){
	uint8_t vHours[EE_HOURS_COUNT];
	uint8_t h = 0;
	
	eeprom_read_block((void*)vHours, (const void*)EE_HOURS_CALIBRATION, EE_HOURS_COUNT);

	h = mLatestDate.hour % 12;
	OCR0A = vHours[h];
	// set PWM for 0% duty cycle	
}
	
#endif /* VOLTMETERDISPLAY_H_ */