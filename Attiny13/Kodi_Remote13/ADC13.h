/*
 * ADC13.h
 *
 * Created: 2016/05/07 18:12:19
 *  Author: Alan
 */ 


#ifndef ADC13_H_
#define ADC13_H_

#include <avr/io.h>

//setup ADC as read on AD2 (PB4) with the max clock prescaler
void setupADC();

//this is the QUICK version, returns the high bits only
uint8_t doADC();


#endif /* ADC13_H_ */