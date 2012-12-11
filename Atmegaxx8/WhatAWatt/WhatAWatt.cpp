/*
 * WhatAWatt.cpp
 *
 * Created: 2012/12/11 23:13:04
 *  Author: Alan
 */ 


#define F_CPU 8000000

#include <avr/io.h>
#include "Rapide7seg.h"

int main(void)
{
	setup7seg();
	
    while(1)
    {
		for (uint16_t vNumber = 0; vNumber < 1000; vNumber++){
			uint16_t vVal = vNumber * 10;
			showNumber(vVal, 0, 2);
		}
    }
}