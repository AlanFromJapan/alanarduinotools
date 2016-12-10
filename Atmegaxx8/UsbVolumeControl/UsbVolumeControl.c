/*
* UsbVolumeControl.c
*
* Created: 2016/12/04 16:29:25
*  Author: Alan
* http://electrogeek.cc/usb%20volume%20control.html
* 
* Based on the works of Vitalij found at http://obruboff.ru/english-pages/usb-volume-control/
*
*/



//defined in the compilation params "-D F_CPU=12000000" or define it here
#define F_CPU 12000000



#include <avr/io.h>
#include <util/delay.h>     // for _delay_ms()
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include "usbdrv.h"
#include "vusb-config.h"


#define VOL_UP		0xe9
#define VOL_DOWN	0xea
#define ROTENC_MASK	0xc0

#define LED_RED		0x02
#define LED_GREEN	0x08
#define LED_BLUE	0x20


static void usbHardwareInit(void) {
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

static inline void ledsOff(){
	PORTD &= ~(LED_BLUE | LED_GREEN | LED_RED);		
}

/************************************************************************/
/* MAIN                                                                 */
/************************************************************************/
int main(void) {

	uchar KeyPressed = 0;

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
	            
	reportBuffer[0] = 1;  // ReportID = 1
	reportBuffer[2] = 0;  
	

	//2: rotary encoder setup (pullups)		
	//PD6-7 input
	DDRD &= ~(ROTENC_MASK);
	//Pull up on PD6-7
	PORTD |= ROTENC_MASK;
		
	//just make sure pullups are NOT disabled
	MCUCR |= (0 << PUD);

	
	//3: LEDS
	DDRD |= LED_BLUE | LED_GREEN | LED_RED;
	//all leds off
	ledsOff();

	
	for(;;){
		usbPoll();
	
	
		if((PIND & ROTENC_MASK) != ROTENC_MASK){
			if ((PIND & ROTENC_MASK) == 0) {
				//ignore, already low on both lines A & B
			
				//all leds off
				ledsOff();
			
				continue;
			}
			else {
				if ((PIND & 0x80) == 0x80) {
					//UP!
					KeyPressed = VOL_UP;
				
					PORTD |= (LED_RED);

				}
				else {
					//down
					KeyPressed = VOL_DOWN;

					PORTD |= (LED_BLUE);
				}
			}
		
			//send the USB message
			if (usbInterruptIsReady()){
				reportBuffer[1] = KeyPressed;
				usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
			
				_delay_ms(10);
			
			
				//and send STOP!!
				KeyPressed = 0x00;
				reportBuffer[1] = KeyPressed;
				usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
			}
		
			//wait a little to debounce on the cheap
			usbDelayMs(20);
		}
		else {
			//other cases: ignore
			KeyPressed = 0x00;
			//all leds off
			ledsOff();
		}

	}
	
	return 0;	
}