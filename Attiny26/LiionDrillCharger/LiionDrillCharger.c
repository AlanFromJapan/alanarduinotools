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

//The current status: "Charging" is when something is plugged and being charged, "Charged" is when unplugged or current plugged one is full
#define STATUS_CHARGING	1
#define STATUS_CHARGED	0

#include <avr/io.h>
#include <util/delay.h>


uint8_t mCurrentStatus = STATUS_CHARGED;


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

	//ALL leds OFF [Pins B6-B3]
	PORTB &= 0b10000111;
	
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
/* Read Li-Ion charger module and update current status and display leds*/
/************************************************************************/
void updateChargerStatusLed(){
	//Read status on A0 & A1 and show on debug pins
	//I put a pullup (external, and now I just remember I could have use the internal one ... anyway)
	
	//README! 
	//inverted logic: if pulled low means transistor of optocoupler is ON means LED on the LiIon charger is ON

	//store in local variable to avoid risk or ready incoherent data 
	uint8_t vPina = PINA;
	//default
	mCurrentStatus = STATUS_CHARGED;
	
	if ((vPina & 0x01) == 0){
		PORTB |= LED_DEBUG1;
	}
	else{
		PORTB &= ~LED_DEBUG1;
	}

	if ((vPina & 0x02) == 0){
		PORTB |= LED_DEBUG2;
		mCurrentStatus = STATUS_CHARGING;
	}
	else{
		PORTB &= ~LED_DEBUG2;
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


void doFinishedAnimation(){

	//ALL leds OFF [Pins B6-B3]
	PORTB &= 0b10000111;

	while(1) {
		for (uint8_t vLed = 2; vLed <= BATTERY_COUNT; vLed++){
			setLEDRelay(vLed, LED_ON);
			_delay_ms(100);
		}
		for (uint8_t vLed = BATTERY_COUNT-1; vLed > 0; vLed--){
			setLEDRelay(vLed, LED_ON);
			_delay_ms(100);
		}	
	}	
}

/*
void test_allRelaysCycle() {
	
	for (uint8_t i = 1; i <=BATTERY_COUNT; i++){
		setRelayOn(i);
		setLEDRelay(i, LED_ON);
	
		_delay_ms(1500);	
	}
}
*/

int main(void)
{
	init();	

	//test_allRelaysCycle();

	uint8_t vCurrentRelay = 1;		

	setRelayOn(vCurrentRelay);
	setLEDRelay(vCurrentRelay, LED_ON);
	
	_delay_ms(100);
	updateChargerStatusLed();
	
	//give some time to the li-ion charger to read the status and update itself
	_delay_ms(2000);

	while (1){		
		updateChargerStatusLed();
		
		//if not charging, assume we're charged or unconnected
		if (mCurrentStatus != STATUS_CHARGING) {
			//go next
			vCurrentRelay++;
			
			if (vCurrentRelay > BATTERY_COUNT){
				//finished, turn all OFF and do some "finished" animation
				setRelayOn(0);
				setLEDRelay(0, LED_OFF);
				doFinishedAnimation();
			}
			else {
				//not finished, turn next relay ON
				setRelayOn(vCurrentRelay);
				setLEDRelay(vCurrentRelay, LED_ON);

				//give some time to the li-ion charger to read the status and update itself
				_delay_ms(2000);
			}				
		}
		
		_delay_ms(200);
	}		
}