/*
 * FingerPasswordKeyboard.c
 *
 * Created: 2015/06/17 22:44:16
 *  Author: Alan
 */ 

//defined in the compilation params "-D F_CPU=12000000"
//#define F_CPU 12000000



#include <avr/io.h>
#include <util/delay.h>     // for _delay_ms()
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>


#include "Keyboard.h"
#include "usbdrv.h"
#include "vusb-config.h"
#include "serialComm.h"
#include "FPS.h"


#include "PasswordConstants.h"
/** NOT in source control because one day there might have my passwords inside.
Contains 4 constants that will be what is send as keyboard upon successful finger press.

Sample:
#define PASSWORDCONSTANT1 "login1\tpassword1\n"
#define PASSWORDCONSTANT2 "login2\tpassword2\n"
#define PASSWORDCONSTANT3 "login3\tpassword3\n"
#define PASSWORDCONSTANT4 "just the password4\n"

**/

static void usbHardwareInit(void)
{
	uint8_t	i, j;

	PORTD = 0xfa;   /* 1111 1010 bin: activate pull-ups except on USB lines */
	DDRD = 0x07;    /* 0000 0111 bin: all pins input except USB (-> USB reset) */
	j = 0;
	while(--j){     /* USB Reset by device only required on Watchdog Reset */
		i = 0;
		while(--i); /* delay >10ms for USB reset */
	}
	DDRD = 0x02;    /* 0000 0010 bin: remove USB reset condition */
	/* configure timer 0 for a rate of 12M/(1024 * 256) = 45.78 Hz (~22ms) */
	//AVI: what is this used for? 
	TCCR0 = 5;      /* timer 0 prescaler: 1024 */
}

static void usbPurgeEvents(){
	do {
		//usb data pull
		wdt_reset();
		usbPoll();
		_delay_ms(3);
	} while (!usbInterruptIsReady());
}

#define USB_DELAY_DELTA_MS 50
static void usbDelayMs(double pDelay){
	uint8_t vDelay = (uint8_t)((uint16_t)pDelay / USB_DELAY_DELTA_MS);
	for (uint16_t t = 0; t < vDelay; t++){
		_delay_ms(USB_DELAY_DELTA_MS);
		//Keep USB connection alive
		usbPurgeEvents();
	}
}
	
//Sends a null terminated string as a keyboard
static void sendString (char* pStr){
	for (uint8_t* p = pStr; (*p) != 0; p++){

		//Necessary: without that if you send a few characters only the first 5-8 ones will arrive
		usbPurgeEvents();
					
		//1) Send the character
		buildKeyboardReport(*p);
		usbSetInterrupt(&keyboard_report, sizeof(keyboard_report));
		_delay_ms(20);
					
		//2) DON'T FORGET to stop sending character FOR EACH Character
		buildKeyboardReport(NULL);
		usbSetInterrupt(&keyboard_report, sizeof(keyboard_report));
		_delay_ms(20);
	}
				
}
	
//defined in FPS.c
extern uint32_t mFPSLatestResponseValue ;
extern uint8_t mFPSLatestResponseStatus ;

//Does the enrollment of a finger and still works as a keyboard
uint8_t DoEnroll( int pID ) 
{
	char vBuf[10];		
	uint8_t v = 0;
	
	sendString("Start\n");
	fpsSetLight(FPS_LIGHT_ON);

	fpsEnrollCheck(pID);
	if (mFPSLatestResponseStatus == FPS_RESPONSE_ACK){
		//it's USED so you CAN'T re-register it -> get out!
		sendString("Used!\n");
		return 10;
	}
	
	//1: enroll start
	fpsEnrollStart(pID);
	if (mFPSLatestResponseStatus != FPS_RESPONSE_ACK){
		sendString("Start KO\n");
		return 15;
	}
	sendString("Start ok\n");

	//2: finger capture
	if (fpsCaptureFinger(123) == 0){
		sendString("Cap1 KO\n");
		return 20;
	}	
	sendString("Cap1 ok\n");
	
	
	//Keep USB connection alive
	usbPurgeEvents();
	
	
	//3: enroll1
	if (fpsEnroll(1) == 0){
		sendString("Enroll1 KO\n");
		return 30;
	}			
	sendString("Enroll1 ok - remove Finger!\n");
	
	////4: wait till finger removed
do {v = fpsIsFingerPressed(); } while (v == 0);
fpsSetLight(FPS_LIGHT_OFF);
//

	_delay_ms(100);


	sendString("Enroll2 start - put finger!\n");
	//5: finger capture
	fpsSetLight(FPS_LIGHT_ON);
	_delay_ms(100);
	if (fpsCaptureFinger(123) == 0){
		sendString("Cap2 KO\n");
		return 40;
	}
	sendString("Cap2 ok\n");

	//Keep USB connection alive
	usbPurgeEvents();

	//6: enroll1
	if (fpsEnroll(2) == 0){
		sendString("Enroll2 KO\n");
		return 50;
	}
	sendString("Enroll2 ok - Finger!\n");

////7: wait till finger removed
	//do {v = fpsIsFingerPressed(); } while (v == 0);
	fpsSetLight(FPS_LIGHT_OFF);
//

	_delay_ms(500);	
	
	//Keep USB connection alive
	usbPurgeEvents();
	
	//8: finger capture
	fpsSetLight(FPS_LIGHT_ON);
	_delay_ms(100);
	if (fpsCaptureFinger(123) == 0){
		sendString("Cap3 KO\n");
		return 60;
	}
	sendString("Cap3 ok\n");

	//Keep USB connection alive
	usbPurgeEvents();
	
	//9: enroll1
	if (fpsEnroll(3) == 0){
		sendString("Enroll3 KO\n");
		return 70;
	}
	sendString("Enroll3 ok - Finger!\n");
	
	fpsSetLight(FPS_LIGHT_OFF);
	_delay_ms(100);
	fpsSetLight(FPS_LIGHT_ON);
	_delay_ms(100);
	fpsSetLight(FPS_LIGHT_OFF);

	sendString("Finished!\n");
}

//////////////////////////////////////////////////////////////////////////
//Returns 0 if not found, [1-19] if ok, other codes are errors
uint8_t fpsIsKnownFinger2 (){
	uint8_t vIdResult;
	
	fpsSetLight(FPS_LIGHT_ON);
	_delay_ms(50);


//Keep USB connection alive
usbPurgeEvents();

	if (fpsCaptureFinger(0) == 0){
		vIdResult = 254;
	}
	else {

//Keep USB connection alive
usbPurgeEvents();
		
		vIdResult = fpsIdentifyFinger();
		//vIdResult = fpsVerifyFinger(1);
	}


//Keep USB connection alive
usbPurgeEvents();

	fpsSetLight(FPS_LIGHT_OFF);
	
	return vIdResult;
}


/************************************************************************/
/* Main method                                                          */
/************************************************************************/
int main(void)
{
    wdt_enable(WDTO_1S);  // watchdog status is preserved on reset

	//1: Init USB
    usbHardwareInit();
    
    usbInit();
    usbDeviceDisconnect();

    // fake USB disconnect for > 250 ms
    for( uint8_t i=255; i>0; i-- ) {
	    wdt_reset();
	    _delay_ms(1);
    }
    usbDeviceConnect();

    sei();

	//2: Now USB is up, init the Serial
	serialHardwareInit();
	
	//3: buttons
	//button input[0;3] 
	//PC[0;3] in
	DDRC &= ~0x0F;
	//Pull up on PC[0;3]
	PORTC = 0x0F;
	//just make sure pullups are NOT disabled
	SFIOR &= ~(1 << PUD);
	
	
	//Vibrator on/off transistor [4] (NPN so active HIGH)
	DDRC |= (1 << DDRC4);
	//C4 low so vibrator is OFF (NPN transistor)
	PORTC &= ~(1 << PORTC4);
	
	
	//FPS on/off transistor [5] (PNP so active LOW)
	DDRC |= (1 << DDRC5);
	//C5 high so FPS is OFF (PNP transistor)
	PORTC |= (1 << PORTC5);


			//Vibrate
			PORTC |= (1 << PORTC4);
			usbDelayMs(200);
			PORTC &= ~(1 << PORTC4);
			usbDelayMs(300);
			PORTC |= (1 << PORTC4);
			usbDelayMs(200);
			PORTC &= ~(1 << PORTC4);
			
    for(;;){                // main event loop
		//USB data pull
		wdt_reset();
		usbPoll();
		
		//Other stuffs to do in main loop
		//check for button press
		uint8_t vPinc = PINC;
		if ((~vPinc & 0x0F) != 0){			
			/*
			//EXAMPLE: send string hardcoded
			uint8_t* str = "hello Monde1230. !\t#$%&'()<>?:;^\n";
			sendString(str);
			*/
					
			/*
			//EXAMPLE: toggle light and return response code
			fpsSetLight(vToggle);
			_delay_ms(200);
			
			//vToggle = !vToggle;
			if (vToggle == 0)
				vToggle = 1;
			else
				vToggle = 0;	
				
			char vBuf[10];
			itoa(mFPSLatestResponseStatus, vBuf, 16);
			sendString(vBuf);
			*/
			
			/*
			//EXAMPLE: check if slot is taken and return error codes
			fpsEnrollCheck(1);
			char vBuf[10];
			itoa(mFPSLatestResponseStatus, vBuf, 16);
			sendString(vBuf);

			sendString(" - ");
			
			uint16_t vRep;
			vRep = (uint16_t)mFPSLatestResponseValue;
			itoa(vRep, vBuf, 16);
			sendString(vBuf);
			//read "31 - 1004" means not used!
			*/
			
			/*
			uint8_t v = fpsEnrollPrintSequence(1);
			char vBuf[10];
			itoa(v, vBuf, 10);
			sendString(vBuf);
			*/
			
			/*
			DoEnroll(1);
			*/
			
			
			//C5 low so FPS is ON (PNP transistor)
			PORTC &= ~(1 << PORTC5);
			//in case voltage needs a little time to stabilize
			_delay_ms(50);

			//Keep USB connection alive
			usbPurgeEvents();
			
			//give a little time to move the finger to the reader (1.5 sec)
			for (uint8_t z = 0; z < 30; z++){
				_delay_ms(50);
				//Keep USB connection alive
				usbPurgeEvents();
			}
			
			
			fpsInit();
			fpsSetLight(FPS_LIGHT_OFF);

			//Keep USB connection alive
			usbPurgeEvents();			
			
			uint8_t v = fpsIsKnownFinger2();

			//Keep USB connection alive
			usbPurgeEvents();
				
			if (v < 1 || v > 19){
				//sendString("UNAUTHORIZED ");
				//char vBuf[10];
				//itoa(v, vBuf, 10);
				//sendString(vBuf);
				//sendString(" !\n");
				
				//Vibrate
				PORTC |= (1 << PORTC4);
				usbDelayMs(300);
				PORTC &= ~(1 << PORTC4);				
			}
			else{
				if ((~vPinc & (1 << PINC0)) != 0){
					sendString(PASSWORDCONSTANT1);
				} 
				else if ((~vPinc & (1 << PINC1)) != 0){
					sendString(PASSWORDCONSTANT2);
				}
				else if ((~vPinc & (1 << PINC2)) != 0){
					sendString(PASSWORDCONSTANT3);
				}
				else if ((~vPinc & (1 << PINC3)) != 0){
					sendString(PASSWORDCONSTANT4);
				}
				else{
					sendString("Unknown button");
				}
			}			
			
			fpsClose();
		
			//Keep USB connection alive
			usbPurgeEvents();
			
			
			//C5 high so FPS is OFF (PNP transistor)
			PORTC |= (1 << PORTC5);
		}		
		
    }	
	
	return 0;
}