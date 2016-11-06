/*
 * LiionDrillCharger.c
 *
 * Created: 2016/10/30 11:00:53
 *  Author: Alan
 *
 * Project page: http://electrogeek.cc/drill%20battery%20from%20laptop%20battery.html
 * Micro controller runs at 1MHz internal oscillator (factory settings)
 *
 */ 

//CPU is 1MHz
#define F_CPU 1000000

#define BATTERY_COUNT	4

#define LED_DEBUG1		0x02
#define LED_DEBUG2		0x04

#define LED_ON			2
#define LED_OFF			0

#include <avr/io.h>
#include <util/delay.h>

/************************************************************************/
/* Turns on ONE relay at a time (only one active possible).             */
/* pRelayId: relay num [1-4]                                            */
/************************************************************************/
void setRelayOn (uint8_t pRelayId){
	//ALL relays OFF [Pins A4-A7]
	PORTA &= 0x0f;
	
	if (pRelayId> 4 || pRelayId == 0)
		return;
	
	//wait just to be sure physically all are disconnected
	//HK4100F-DC5V-SHG doc says "release time = max 5ms", so let's be on the large side
	_delay_ms(50);
	
	//relay ID [0-3]
	pRelayId = pRelayId -1;
	//pin ID [A7=Relay1, A4=Relay4]
	pRelayId = 7 - pRelayId;
	//Set Relay ON
	PORTA |= (1 << pRelayId);
}



/************************************************************************/
/* Turns on ONE LED (of relay) at a time                                */
/* pRelayId: relay num [1-4]                                            */
/* pFlagOnOff: 0 = turn off, other = turn on                            */
/************************************************************************/
void setLEDRelay (uint8_t pRelayId, uint8_t pFlagOnOff){
	if (pRelayId> 4 || pRelayId == 0)
		return;	

	//Led pin [PB6= Relay1, PB3= Relay4]
	pRelayId = 7 - pRelayId;
	
	if(pFlagOnOff == 0){
		//turn OFF
		PORTB &= ~ (1 << pRelayId);
	}
	else {
		//turn ON
		PORTB |= (1 << pRelayId);
	}
}
	
	
/************************************************************************/
/* Init function. Call me first.                                        */
/************************************************************************/	
void init(){
	//B is all out
	DDRB = 0xff;
	//A is in for A0-1, out for the rest
	DDRA = 0xfc;

	//All leds off, all relays off	
	PORTB = 0x00;
	PORTA = 0x00;	
}


int main(void)
{
	init();	

	//force turn on the #1 after 10 sec
	_delay_ms(3000);
	setRelayOn(1);
	setLEDRelay(1, LED_ON);

	//Read status on A0 & A1 and show on debug pins
	//I put a pullup (external, and now I just remember I could have use the internal one ... anyway)
	while (1){
		
		//inverted logic: if pulled low means transistor of optocoupler is ON means LED on the LiIon charger is ON
		if ((PINA & 0x01) == 0){
			PORTB |= LED_DEBUG1;
		}
		else{
			PORTB &= ~LED_DEBUG1;			
		}

		if ((PINA & 0x02) == 0){
			PORTB |= LED_DEBUG2;
		}
		else{
			PORTB &= ~LED_DEBUG2;
		}
		
		_delay_ms(50);
	}		
}