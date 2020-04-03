/*
 * GbCartridgeKSController.cpp
 *
 * Created: 2015/02/05 23:35:27
 *  Author: Alan
 */ 


//#define F_CPU 16000000	// 16 MHz oscillator.


#define TALKATIVE
#define SERIAL_ENABLED


#include <avr/io.h>
#include <util/delay.h>
//#include <avr/interrupt.h>
#include <stddef.h>
#include <stdlib.h>

#include "serial_utils.h"
#include "flash_utils.h"

#include "tests/unit_tests.h"



void showHelp(){
	serialWriteString("Help: \n");
	serialWriteString("   rAAAA : gets the byte at 0xAAAAA\n");
	serialWriteString("   wAAAADD : writes the value 0xDD at 0xAAAAA\n");
	serialWriteString("   d : shows data port status\n");
	serialWriteString("   ? : print this help\n");
}


/************************************************************************/
/* Shows content of dataport                                            */
/************************************************************************/

#ifdef TALKATIVE
void showDataPort() {
	serialWriteString("Data port content: PORTA=0x");

	char vBuffChar[6];
	serialWriteString(itoa(PORTA, vBuffChar, 16));

	serialWriteString(" PINA=0x");
	serialWriteString(itoa(PINA, vBuffChar, 16));

	serialWriteString("\n");
}
#endif //TALKATIVE

/************************************************************************/
/* DECODE command                                                       */
/************************************************************************/
void decodeCommand (){	
	uint8_t inByte = 0;         // incoming serial byte
	
	if (serialCheckRxComplete()) {
		inByte = serialRead();
			
		switch (inByte)
		{
			case 'r':
			case 'R':
				//GET a byte
				//Format "rAAAA" with AAA is address MSB first
				flashGetByteDecode();
				break;

			case 'w':
			case 'W':
				//WRITE a byte
				//Format "wAAAADD" with AAA is address and DD is data (MSB first)
				flashWriteByteDecode();
				break;

			case '?':
				showHelp();
				break;

#ifdef TALKATIVE
			case 'n':
				//GET a byte for the test
				flashGetByteDecode2(0xffff);
				break;
			case 'm':
				//WRITE a byte for the test
				flashWriteByteDecode2(0xffff, 0x67);
				break;

			case 'd':
			case 'D':
				//show content of data port
				showDataPort();
				break;

#endif //TALKATIVE
			case 't':
				//run tests
				test1(100);
				break;
			case 'Z':
				//delete everything
				flashReset();
				flashErase();
				flashReset();
				serialWriteString("Erase all completed.\n");
				break;
			default:
			//Unknown command
#ifdef TALKATIVE
			serialWriteString("ERROR: unknown command: ");serialWrite(inByte);serialWriteString("\n");
#endif //TALKATIVE				
			break;
		}
	}
}

/************************************************************************/
/*  MAIN                                                                */
/************************************************************************/
int main (void)
{
	
	//Interrupts are not needed in this program. You can optionally disable interrupts.
	//asm("cli");		// DISABLE global interrupts.

#ifdef SERIAL_ENABLED		
	setupSerial();
#endif //SERIAL_ENABLED	

	setupExternalFlash();
	

	establishContact();  // send a byte to establish contact until Processing responds
	
#ifdef TALKATIVE
	showHelp();
#endif //TALKATIVE


	//for (uint8_t i = 0; i < 255; i++){
		//flashWriteByteDecode2((uint16_t)i, i);		
	//}
	//

	while(1) {
#ifdef SERIAL_ENABLED	
		if (serialCheckRxComplete()) {
			decodeCommand();
#endif //SERIAL_ENABLED
			

#ifdef SERIAL_ENABLED
		}
#endif //SERIAL_ENABLED
	}	//End main loop.
	return 0;
}
