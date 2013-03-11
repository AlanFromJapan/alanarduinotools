/*
* RGBMatrix.cpp
*
* Created: 2013/01/05 18:54:14
*  Author: Alan
*/

//well in real it's more like 8MHz but this value makes a nice flickerless display so...
#define F_CPU 1000000L

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define POV_ON_US 300
#define POV_OFF_US 5000

void AllOff (){
	PORTD = 0xff;
	PORTE = 0xff;
	
	PORTC = 0x0f; PORTA = 0xf0;
}

volatile uint8_t mBlueMatrix	[8];
volatile uint8_t mRedMatrix		[8];
volatile uint8_t mGreenMatrix	[8];

void setMatrix(uint8_t pRGB, uint8_t pX, uint8_t pY, uint8_t pValue){
	if (pRGB == 'B') {
		mBlueMatrix[pY] = (pValue == 0 ? mBlueMatrix[pY] & ~(1 << pX) : mBlueMatrix[pY] | (1 << pX));
	}	
	if (pRGB == 'R') {
		mRedMatrix[pY] = (pValue == 0 ? mRedMatrix[pY] & ~(1 << pX) : mRedMatrix[pY] | (1 << pX));
	}
	if (pRGB == 'G') {
		mGreenMatrix[pY] = (pValue == 0 ? mGreenMatrix[pY] & ~(1 << pX) : mGreenMatrix[pY] | (1 << pX));
	}
}


void showMatrix(){
	PORTE = 0xff;
	PORTA = 0xf0;
	PORTC = 0x0f;
	
	for (int y = 0; y < 8; y++){
		uint8_t vBlue = mBlueMatrix[y];
		
		//set the line on
		if (y < 4){
			PORTC = (1 << (7-y)) | 0x0f;
			PORTA = 0xf0;
		}
		else {
			//y >= 4
			PORTA = (1 << (7-y)) | 0xf0;
			PORTC = 0x0f;
		}
		

		//blue is simply PORTD
		PORTD = ~vBlue;
									
		_delay_us(POV_ON_US);
		//all off
		PORTD = 0xff;
		_delay_us(POV_OFF_US);
	
	}	


	for (int y = 0; y < 8; y++){
		uint8_t vGreen = mGreenMatrix[y];
		
		//set the line on
		if (y < 4){
			PORTC = (1 << (7-y)) | 0x0f;
			PORTA = 0xf0;
		}
		else {
			//y >= 4
			PORTA = (1 << (7-y)) | 0xf0;
			PORTC = 0x0f;
		}
		
		//green is shared on PORTA4-7 & PORTC0-3
		vGreen = ~vGreen; //negative logic
		PORTA = (vGreen << 4) | (PORTA & 0x0f);
		PORTC = (vGreen >> 4) | (PORTC & 0xf0);
		
		_delay_us(POV_ON_US);
		//all off	
		PORTA = 0xf0;
		PORTC = 0x0f;
		_delay_us(POV_OFF_US);
		
	}
	
	
	for (int y = 0; y < 8; y++){
		uint8_t vRed = mRedMatrix[y];
			
		//set the line on
		if (y < 4){
			PORTC = (1 << (7-y)) | 0x0f;
			PORTA = 0xf0;
		}
		else {
			//y >= 4
			PORTA = (1 << (7-y)) | 0xf0;
			PORTC = 0x0f;
		}
			
			
		//red is PORTE
		PORTE = ~vRed;
			
		_delay_us(POV_ON_US);
		//all off
		PORTE = 0xff;
		_delay_us(POV_OFF_US);
			
	}
	
	PORTE = 0xff;
	PORTA = 0xf0;
	PORTC = 0x0f;
}


ISR(TIMER2_OVF_vect){
	//setMatrix('R', 0, 0, 1);
	//mRedMatrix[0] = 0xff;
}

//inits timer 1 to do interrupt on overflow (calls ISR(TIMER2_OVF_vect))
void init_timer2_OVF() {
	
	//timer 2
	TCCR2 = 
		//No PWM
		(1 << FOC2) 
		//Divide by 1024
		| (1 << CS22) | (1 << CS20);
	
	//trigger the interrupt vector TIMER1_OVF_vect when timer 1 is overflow
	TIMSK = (1 << TOV2);
	
	//sets the interruptions to enabled
	sei();
}


int main(void)
{
	DDRD = 0xFF;
	DDRE = 0xFF;
	DDRA = 0xFF;
	DDRC = 0xFF;
	
	//A0-A3 : are the negative row 1-4
	//C4-C7 : are the negative row 5-8
	
	XDIV = 0x00;

	//init_timer2_OVF();
	
	while (1){
	
		mGreenMatrix[rand()%8] = (uint8_t)rand();	
		mRedMatrix[rand()%8] = (uint8_t)rand();
		mBlueMatrix[rand()%8] = (uint8_t)rand();
		
		for (uint8_t i = 0; i < 30; i++){
			showMatrix();
		}	
				
	}
	
}