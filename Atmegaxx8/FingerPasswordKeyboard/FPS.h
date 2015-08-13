/*
 * FPS.h
 *
 * Created: 2015/07/13 22:08:39
 *  Author: Alan
 */ 


#ifndef FPS_H_
#define FPS_H_


#include <avr/io.h>

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

#endif /* FPS_H_ */