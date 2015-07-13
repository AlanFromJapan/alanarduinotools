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

#endif /* FPS_H_ */