/*
  twi.h - TWI/I2C library 
  
  http://www.embedds.com/programming-avr-i2c-interface/
  */

#ifndef twi_h
#define twi_h

#include "util/twi.h"


void TWIInit(void);

void TWIStart(void);
//send stop signal
void TWIStop(void);

void TWIWrite(uint8_t u8data);

uint8_t TWIReadACK(void);
//read byte with NACK
uint8_t TWIReadNACK(void);

uint8_t TWIGetStatus(void);

#endif

