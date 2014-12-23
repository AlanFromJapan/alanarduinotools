/* Name: main.c
 * Project: HID-Test
 * Author: Christian Starkjohann
 * Creation Date: 2006-02-02
 * Tabsize: 4
 * Copyright: (c) 2006 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt) or proprietary (CommercialLicense.txt)
 * This Revision: $Id$
 */

#define F_CPU 12000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include "usbdrv.h"
#include "Keyboard.h"
#include "vusb-config.h"

/* ----------------------- hardware I/O abstraction ------------------------ */

/* pin assignments:
PB0	Key 1
PB1	Key 2
PB2	Key 3
PB3	Key 4
PB4	Key 5
PB5 red LED
PB6 yellow LED
PB7	green LED

PA0 
PA1

PD0	USB-
PD1	debug tx
PD2	USB+ (int0)
PD3	Key 13
PD4	Key 14
PD5	Key 15
PD6	Key 16
*/

static void hardwareInit(void)
{
uchar	i, j;

    PORTB = 0x1f;   /* activate all pull-ups EXCEPT on leds */
    DDRB = 0xe0;       /* all pins input EXCEPT on leds that are outputs */
    
    PORTD = 0xfa;   /* 1111 1010 bin: activate pull-ups except on USB lines */
    DDRD = 0x07;    /* 0000 0111 bin: all pins input except USB (-> USB reset) */
	j = 0;
	while(--j){     /* USB Reset by device only required on Watchdog Reset */
		i = 0;
		while(--i); /* delay >10ms for USB reset */
	}
    DDRD = 0x02;    /* 0000 0010 bin: remove USB reset condition */
    /* configure timer 0 for a rate of 12M/(1024 * 256) = 45.78 Hz (~22ms) */
    TCCR0A = 5;      /* timer 0 prescaler: 1024 */
}

/* ------------------------------------------------------------------------- */

#define NUM_KEYS    17




const PROGMEM uint16_t mMappingCharKeys[] = {
	('a' << 8) | KEY_A,
	('b' << 8) | KEY_B,
	('c' << 8) | KEY_C,
	('d' << 8) | KEY_D,
	
	};
	
char mText[] = "aadbc";
uint8_t mTextPos = 0;


void buildReport2(char key)
{
	if (key == 0){
		reportBuffer[0] = 0;
		reportBuffer[1] = 0;
	}
	
	uint16_t vWord = 0x0000;
	for (uint8_t i= 0; i < 4; i++){
		vWord = pgm_read_word(&(mMappingCharKeys[i]));
		//vWord = mMappingCharKeys[i];
		
		//if the key is good, return the key
		if ((vWord >> 8) == key){
			reportBuffer[0] = 0;
			reportBuffer[1] = (uint8_t)vWord;			
			return;
		}
	}

	//if you arrive here, char was not found
	reportBuffer[0] = 0;
	reportBuffer[1] = 0;

}

/* ------------------------------------------------------------------------- */

int	main(void)
{
uchar   key, lastKey = 0, keyDidChange = 0;
uchar   idleCounter = 0;

//uint8_t vKars[] = {KEY_W, KEY_X, KEY_Y, KEY_Z}; uint8_t vKarPos = 0;



	wdt_enable(WDTO_2S);
    hardwareInit();
//	odDebugInit();
	usbInit();
	sei();
   // DBG1(0x00, 0, 0);
	for(;;){	/* main event loop */
		wdt_reset();
		usbPoll();
        key = keyPressed();
        if(lastKey != key){
            lastKey = key;
            keyDidChange = 1;
        }
        if(TIFR & (1<<TOV0)){   /* 22 ms timer */
            TIFR = 1<<TOV0;
            if(idleRate != 0){
                if(idleCounter > 4){
                    idleCounter -= 5;   /* 22 ms in units of 4 ms */
                }else{
                    idleCounter = idleRate;
                    keyDidChange = 1;
                }
            }
        }
        if(keyDidChange && usbInterruptIsReady()){
            keyDidChange = 0;
            /* use last key and not current key status in order to avoid lost
               changes in key status. */
            //buildReport(lastKey);
			
			buildReport2((lastKey == 0 ? lastKey : mText[mTextPos++])); if (mTextPos >= sizeof(mText)) mTextPos = 0;
			
			//reportBuffer[1] = vKars[vKarPos++]; if (vKarPos >= sizeof(vKars)) vKarPos = 0;
			
            usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
        }
	}
	return 0;
}

/* ------------------------------------------------------------------------- */
