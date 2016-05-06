/*
 * KodiRemote_Receiver.c
 *
 * Created: 2016/02/23 19:40:07
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
#include "Adc.h"

#include "FiniteStateMachine.h"

#define WIRE_DEBOUNCE_COUNT 3
#define WIRE_DEBOUNCE_MS	15

uint8_t FSM_PATTERN[] = {0xAA, 0x01, 0xff, 0x81};
#define FSM_PATTERN_LEN 4

fsm fsm1;

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
	return;
	do {
		//usb data pull
		wdt_reset();
		usbPoll();
		_delay_ms(3);
	} while (!usbInterruptIsReady());
}

#define USB_DELAY_DELTA_MS 10
static void usbDelayMs(double pDelay){
	uint8_t vDelay = (uint8_t)((uint16_t)pDelay / USB_DELAY_DELTA_MS);
	for (uint16_t t = 0; t < vDelay; t++){
		_delay_ms(USB_DELAY_DELTA_MS);
		//Keep USB connection alive
		usbPurgeEvents();
	}
	
	//delay\ms needs a constant so this doesn't work
	//_delay_ms(vDelay % USB_DELAY_DELTA_MS);
	////Keep USB connection alive
	//usbPurgeEvents();
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
	
	
uint8_t readWirelessDebounced() {
	//read x times, return 1 if read ONES *only*
	
	uint8_t vValue = 1;
	
	for (uint8_t i = 0; i < WIRE_DEBOUNCE_COUNT; i++){
		vValue &= (PINC & 0x01);
		usbDelayMs(WIRE_DEBOUNCE_MS);
	}
	
	return vValue;
}
	
	
//uint8_t readWirelessDebouncedStrict() {
	////read x times, return 1 if read ONES *only*, 0 if only zeroes, 2 otherwise
		//
	//uint8_t vValue = 0;
	//uint8_t vGotZeroes = 0;
		//
	//for (uint8_t i = 0; i < WIRE_DEBOUNCE_COUNT; i++){
		//uint8_t v = (PINC & 0x01);
		//
		//vValue |= v;
		//vGotZeroes |= (0xfe & ~v) todo fixer ca demain;
				//
		//usbDelayMs(WIRE_DEBOUNCE_MS);
	//}
		//
	//return vValue;
//}
	//




/************************************************************************/
/* Main method                                                          */
/************************************************************************/
int main(void)
{
	
/*	
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
*/	
	//3: Leds
	DDRB = 0xff; //PortB all output
	
	//4: ADC init on ADC0 -> PC0
	//InitADC(0x00);
	
	//Port C in
	DDRC = 0x00;
		
	//Init the finite state machine
	fsmInit(&fsm1, FSM_PATTERN, FSM_PATTERN_LEN);
		
	uint8_t vWireless = 0;
	for(;;){    
		//pulse red
		PORTB = 0x04;
		            
		//Keep USB connection alive
		usbPurgeEvents();

		//vWireless = ReadADCx8();
			//
		//if (vWireless > 200) {
			//PORTB = 0x01;
		//}
		//else {
			//PORTB = 0x00;
		//}
		
		
		
		//if ((PINC & 0x01) != 0x00) {
			//PORTB = 0x01;
		//} 
		//else {
			//PORTB = 0x00;
		//}
		
		
		
		//if (readWirelessDebounced() == 0x01) {
			//PORTB = 0x01;
		//}
		//else {
			//PORTB = 0x00;
		//}
		
		uint8_t vRead = 1;

		vRead = vRead * (PINC & 0x01);
		_delay_ms(1);
		vRead = vRead * (PINC & 0x01);
		_delay_ms(1);
		vRead = vRead * (PINC & 0x01);
		_delay_ms(1);

		fsmTransit(&fsm1, vRead);
		
		////read pin
		//if ((PINC & 0x01) != 0x00) {
			//fsmTransit(&fsm1, 1);
		//}
		//else {
			//fsmTransit(&fsm1, 0);			
		//}

		if (fsmIsCompleted(&fsm1) == 1){
			
			PORTB = 0x01;
			usbDelayMs(200);
			PORTB = 0x00;
			fsmReset(&fsm1);
			
			usbDelayMs(1000);
		}


		usbDelayMs(10);
		PORTB = 0x00;
	}	
	
	return 0;
}