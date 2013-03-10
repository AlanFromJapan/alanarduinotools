/*
* RGBMatrix.cpp
*
* Created: 2013/01/05 18:54:14
*  Author: Alan
*/


#include <avr/io.h>
#include <util/delay.h>

#define POV_ON_US 500
#define POV_OFF_US 1500

void AllOff (){
	PORTD = 0xff;
	PORTE = 0xff;
	
	PORTC = 0x0f; PORTA = 0xf0;
}

void SwipeAll4() 
{
		while(1)
	{
		for (uint8_t a = 0; a <= 0xff; a++)
		{
			for (uint8_t c = 0; c <= 0xff; c++)
			{
				for (uint8_t e = 0; e <= 0xff; e++)
				{
					for (uint8_t d = 0; d <= 0xff; d++)
					{
						PORTA = a;
						PORTC = c;
						PORTD = d;
						PORTE = e;
						_delay_us(POV_ON_US);

						AllOff();
						_delay_us(POV_OFF_US);
						
					}
					
				}
				
			}
			
		}
	}
}


uint8_t mBlueMatrix [8];
uint8_t mRedMatrix [8];
uint8_t mGreenMatrix [8];

void setMatrix(uint8_t pRGB, uint8_t pX, uint8_t pY, uint8_t pValue){
	mBlueMatrix[pY] = (pValue == 0 ? mBlueMatrix[pY] & ~(1 << pX) : mBlueMatrix[pY] | (1 << pX));
}


void showMatrix(){
	PORTE = 0xff;
	PORTA = 0xf0;
	PORTC = 0x0f;
	
	for (int y = 0; y < 8; y++){
		uint8_t pValue = mBlueMatrix[y];
		
		if (y < 4){
			PORTC = (1 << (7-y)) | 0x0f;
			PORTA = 0xf0;
		}
		else {
			//y >= 4
			PORTA = (1 << (7-y)) | 0xf0;
			PORTC = 0x0f;
		}
		

		//blue is simply PORTB					
		PORTD = ~pValue;
					
		_delay_us(POV_ON_US);
		//Blue all off
		PORTD = 0xff;
		_delay_us(POV_OFF_US);
	
	}	
}

void Light0x0() 
{
		
	uint8_t i = 0;
	while(1)
	{
		//PORTA = 0x01; PORTE = 0x01;
		
		//PORTE = 0x01; PORTA = 0x0e;
		
		PORTD = ~0x80; PORTE = 0xff; PORTC = 0x8f; PORTA = 0xf0;
		
		_delay_us(POV_ON_US);

		AllOff();
		_delay_us(POV_OFF_US);
	}
}

int main(void)
{
	DDRD = 0xFF;
	DDRE = 0xFF;
	DDRA = 0xFF;
	DDRC = 0xFF;
	
	//A0-A3 : are the negative row 1-4
	//C4-C7 : are the negative row 5-8
	
	//	AllOff();


	XDIV = 0x00;

	//SwipeAll4();
	//Light0x0();

/*	
	uint8_t c = 0;
	setMatrix('B', c, c, 1);c++;
	setMatrix('B', c, c, 1);c++;
	setMatrix('B', c, c, 1);c++;
	setMatrix('B', c, c, 1);c++;
	setMatrix('B', c, c, 1);c++;
	setMatrix('B', c, c, 1);c++;
	setMatrix('B', c, c, 1);c++;
	//setMatrix('B', c, c, 1);c++;
*/	
	/*
	mBlueMatrix[0] = 0b00000101;
	mBlueMatrix[2] = 0b00001010;
	mBlueMatrix[6] = 0b00010100;
	*/
/*
	mBlueMatrix[0] = 0b00000001;
	mBlueMatrix[1] = 0b00000010;
	mBlueMatrix[2] = 0b00000100;
	mBlueMatrix[3] = 0b00001000;
	mBlueMatrix[4] = 0b00010000;
	mBlueMatrix[5] = 0b00100000;
	mBlueMatrix[6] = 0b01000000;
	//mBlueMatrix[7] = 0b10000000;
*/


	mBlueMatrix[5] = 0b11100000;
	mBlueMatrix[6] = 0b11000000;
	//mBlueMatrix[7] = 0b11000000;
	
	mGreenMatrix[0] = 0b00000001;
	
	while (1){
		
		
		showMatrix();
/*		
		for (uint8_t i = 0; i < 7; i++){
			//setMatrix('B', i % 8, i / 8, 1);
			uint8_t x = i;
			uint8_t y = i;
		
			//setMatrix('B', x, y, 1);
		
			showMatrix();
		}		
		//i = (i >= 64 ? 0 : i+1);
*/		
	}
	
}