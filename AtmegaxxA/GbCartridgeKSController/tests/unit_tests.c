/*
 * unit_tests.c
 *
 *  Created on: Mar 10, 2020
 *      Author: alan
 */

#include <avr/io.h>
#include <util/delay.h>
//#include <avr/interrupt.h>
#include <stddef.h>
#include <stdlib.h>

#include "unit_tests.h"
#include "../serial_utils.h"
#include "../flash_utils.h"

//Write n values, read them, print serial the result
void test1(uint8_t n){

	uint16_t vAddr = 0x0000;
	uint8_t vVal = 0+1;

	//write
	for (uint8_t i = 0; i < n; i++){
		flashWriteByteDecode2(vAddr, vVal);
		vAddr++; vVal++;
	}

	//reads
	test2(n);
}

//Read n values, print serial the result
void test2(uint8_t n){

	uint16_t vAddr = 0x0000;
	uint8_t ok = 0;

	vAddr = 0x0000;
	//read
	for (uint8_t i = 0; i < n; i++){
		if ((i+1) == flashGetByteDecode2(vAddr++))
			serialWriteString("ok\n");
		else {
			ok = 1;
			serialWriteString("** ERROR !!!***\n");
		}
	}

	if (ok == 0){
		serialWriteString("** Test result is ok ***\n");
	}
	else {
		serialWriteString("** Test result is ERROR!!!  ***\n");
	}

}
