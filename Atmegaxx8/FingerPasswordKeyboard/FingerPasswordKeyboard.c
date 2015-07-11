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

#include "usbdrv.h"
#include "Keyboard.h"
#include "vusb-config.h"
#include "serialComm.h"

static void usbHardwareInit(void)
{
	uint8_t	i, j;

	PORTB = 0x00;   //no pullups
	DDRB = 0xff;    //all outputs

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


void fpsSend(uint16_t pParam, uint8_t pCommand){
	//https://github.com/mlaws/GT-511C1_Mega/blob/master/fingerprint.ino
	uint16_t vChecksum = 0;
	vChecksum = 0x55+0xAA+0x01+(uint8_t)pParam+((uint8_t)(pParam >> 8))+pCommand;
	
	USART_Transmit((uint8_t)0x55); //Command start code 1
	USART_Transmit((uint8_t)0xAA); //Command start code 2
	USART_Transmit((uint8_t)0x01); // This is the first byte for the device ID. It is the word 0x0001
	USART_Transmit((uint8_t)0x00); // Second byte of Device ID. Notice the larger byte is first. I'm assuming this is because the datasheet says "Multi-byte item is represented as Little Endian"
	USART_Transmit((uint8_t)pParam); //writing the largest byte of the Parameter
	USART_Transmit((uint8_t)(pParam >> 8)); //Writing the second largest byte of the Parameter
	USART_Transmit((uint8_t)0x00); //The datasheet says the parameter is a DWORD, but it never seems to go over the value of a word
	USART_Transmit((uint8_t)0x00); //so I'm just sending it a word of data. These are the 2 remaining bytes of the Dword
	USART_Transmit((uint8_t)pCommand); //write the command byte
	USART_Transmit((uint8_t)0x00); //again, the commands don't go over a byte, but it is sent as a word, so I'm only sending a byte
	USART_Transmit((uint8_t)vChecksum); //Writes the largest byte of the checksum
	USART_Transmit((uint8_t)(vChecksum >> 8)); //writes the smallest byte of the checksum
}



int main(void)
{
    wdt_enable(WDTO_1S);  // watchdog status is preserved on reset

	//Init USB
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

	//Now USB is up, init the Serial
	serialHardwareInit();
	

	//open communication to FPS
	fpsSend(0x0000,0x01);
	
	uint32_t vSpeedDown = 0;
	uint8_t vToggle = 0;
    for(;;){                // main event loop
		wdt_reset();
		usbPoll();
		
		//Serial test
		//_delay_ms(100);
		//USART_Transmit('T');
		
		
		vSpeedDown++;		
		if (vSpeedDown >= 500000){
			vSpeedDown = 0;
			
			fpsSend((uint16_t)vToggle,0x12);
			
			//vToggle = !vToggle;
			if (vToggle == 0)
				vToggle = 1;
			else
				vToggle = 0;
		}
		
    }	
	
	return 0;
}