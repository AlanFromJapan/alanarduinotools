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

//Contains the calibration for the minutes
#define EE_MINUTES_COUNT 7
uint8_t EEMEM EE_MINUTES_CALIBRATION[EE_MINUTES_COUNT];

//Store the latest date received
Date mLatestDate;

#define VOLTMETER_PWM_PIN_HOURS 0x20  /*= pin D5 */
#define VOLTMETER_PWM_PIN_MINUTES 0x40  /*= pin D6 */

/************************************************************************/
/* Setup for the PWM outputs                                            */
/************************************************************************/
void voltmeterSetupPWM(){
	
	DDRD |= (VOLTMETER_PWM_PIN_HOURS | VOLTMETER_PWM_PIN_MINUTES);
	// PD6 is now an output

	OCR0A = 0;
	OCR0B = 0;
	// set PWM for 0% duty cycle


	TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
	// set none-inverting mode

	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	// set fast PWM Mode

	TCCR0B |= (1 << CS00);
	// NO PRESCALER (or the coil of the voltmeter will doing horrible high pitched sound) and starts PWM


}
	
/************************************************************************/
/* stores values in memory once                                         */
/************************************************************************/
void voltmeterSetup() {	
	uint8_t vHours[EE_HOURS_COUNT];
	uint8_t vMinutes[EE_MINUTES_COUNT];
	
	voltmeterSetupPWM();
	
	eeprom_read_block((void*)vHours, (const void*)EE_HOURS_CALIBRATION, EE_HOURS_COUNT);
	
	//will program ONCE if last value = 0, otherwise leave what is there (unprogrammed eeprom block read 0xff)
	if (0xff == vHours[0]){
		//my results with a 1kOhm resistor and my hours voltmeter, barely used
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


	eeprom_read_block((void*)vMinutes, (const void*)EE_MINUTES_CALIBRATION, EE_MINUTES_COUNT);
	
	//will program ONCE if last value = 0, otherwise leave what is there (unprogrammed eeprom block read 0xff)
	if (0xff == vMinutes[0]){
		//my results with a 750 Ohm resistor and my minutes voltmeter, barely used
		vMinutes[0] = 0;	//00 min
		vMinutes[1] = 30;	//10 min
		vMinutes[2] = 55;	//20 "
		vMinutes[3] = 82;	//30 "
		vMinutes[4] = 120;	//40 "
		vMinutes[5] = 175;	//50 "
		vMinutes[6] = 255;	//60 " (trick so that there's always a x < minutes < x+1
		
		eeprom_update_block((const void*)vMinutes, (void*)EE_MINUTES_CALIBRATION, EE_MINUTES_COUNT);
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
	uint8_t vMinutes[EE_MINUTES_COUNT];
	uint8_t vBase = 0, vDifference = 0;
	
	//read array, get the stored pwm value and set the PWM
	eeprom_read_block((void*)vHours, (const void*)EE_HOURS_CALIBRATION, EE_HOURS_COUNT);
	OCR0B = vHours[mLatestDate.hour % 12];
	
	
	//little more subtle: read the array, put it proportionally to the past minute, and then affect
	eeprom_read_block((void*)vMinutes, (const void*)EE_MINUTES_CALIBRATION, EE_MINUTES_COUNT);			
	vBase = vMinutes[mLatestDate.minute /10];
	vDifference = vMinutes[1 + (mLatestDate.minute /10)] - vBase;
	//lost of precision but works with uint8_t
	vDifference = (vDifference / 10) * (mLatestDate.minute % 10);
	vBase += vDifference;
	OCR0A = vBase;
	
}
	
#endif /* VOLTMETERDISPLAY_H_ */