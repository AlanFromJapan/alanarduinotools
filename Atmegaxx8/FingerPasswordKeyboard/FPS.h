/*
 * FPS.h
 *
 * Created: 2015/07/13 22:08:39
 *  Author: Alan
 */ 


#ifndef FPS_H_
#define FPS_H_


#include <avr/io.h>


#define FPS_LIGHT_ON		1
#define FPS_LIGHT_OFF		0
#define FPS_RESPONSE_ACK	0x30
#define FPS_RESPONSE_NACK	0x31


//open communication to FPS
void fpsInit();
//turns light on/off
void fpsSetLight (uint8_t pValue);
//closes communication to FPS
//Doc says it does nothing, so is it worth using?
void fpsClose();
//Check enrollment of ID [0;19]
//ACK means enrolled, NACK -> check if response is not used
void fpsEnrollCheck(uint8_t pID);
//Full print sequence enrollment
//uint8_t fpsEnrollPrintSequence (uint8_t pID);
//Returns 0 if not found, [1-19] if ok, other codes are errors
uint8_t fpsIsKnownFinger ();

#endif /* FPS_H_ */