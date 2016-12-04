/*
 * UsbVolumeControl.c
 *
 * Created: 2016/12/04 16:29:25
 *  Author: Alan
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

int main(void)
{


   uchar encstate;
    uchar Btnstate = 0;
    uchar LastBtnstate = 0;
	uchar LastKeyPress = 0;
	uchar KeyPressed = 0;
// 	odDebugInit();
//	ENC_InitEncoder();


//1: Init USB
usbHardwareInit();

    usbInit();
    usbDeviceDisconnect();  
    uchar i = 0;
    while(--i){             
        _delay_ms(1);

    }
    
    usbDeviceConnect();     

    sei();                  
	reportBuffer[0] = 1;  // ReportID = 1
	reportBuffer[2] = 0;  
    DBG1(0x00, 0, 0);
	
		//2: buttons
		//button input
		//PC1 in
		DDRC &= ~((1 << PORTC1) | (1 << PORTC3));
		//Pull up on PC1
		PORTC = (1 << PORTC1) | (1 << PORTC3);
		//just make sure pullups are NOT disabled
		MCUCR |= (0 << PUD);
	
	for(;;){                
      usbPoll();          
//      ENC_PollEncoder();
	  
      ///////////////////////////////////////////////
      
	  //down
      KeyPressed = 0xea;
	  
	  /*
	  encstate = ENC_GetStateEncoder();
      if (LEFT_SPIN == encstate)
      {
         KeyPressed = 0xea;
      }
      else if (RIGHT_SPIN == encstate)
      {
         KeyPressed = 0xe9;
      }
	  Btnstate = ENC_GetBtnState();
	  if (Btnstate != LastBtnstate)
	  {
	    if (Btnstate != 1) KeyPressed = 0xe2;				
		LastBtnstate = Btnstate;
	  }	
        
		*/
	  if((~PINC & (1 << PINC3)) != 0){
		 DBG1(0x01, reportBuffer, 3);
		 if (usbInterruptIsReady()){
			LastKeyPress = KeyPressed;
			reportBuffer[1] = KeyPressed;
			DBG1(0x01, reportBuffer, 3);
			/* use last key and not current key status in order to avoid lost
             changes in key status. */
			usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
		 }	
		/* This block sets the the number of additional keypress a volume key. 
		This increases the rate of change of volume of from 2 to 50 times
		The number of additional keypress sets by the variable AdditionalKeyPress. 	*/
		uchar AdditionalKeyPress = 0;
		while(AdditionalKeyPress--){ 
			if ((KeyPressed == 0xea)||(KeyPressed == 0xe9)){
				while (!(usbInterruptIsReady())){}
				usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
			}
		}//End of block		
		
		usbDelayMs(330);
      }
	}
    return 0;
	
}