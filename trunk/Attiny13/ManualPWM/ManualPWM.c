/*
 * ManualPWM.c
 *
 * Created: 2012/02/13 0:08:35
 *  Author: Alan
 */ 


#define F_CPU 8000000L
#define READING_BUFFER 4
#define LIMIT_BLUEGREEN 12 
#define LIMIT_GREENRED 23
#define LIMIT_DELTA 4

#include <avr/io.h>
#include <util/delay.h>

uint8_t mRGB[3];
uint16_t mRawADCBuffer[READING_BUFFER];
uint8_t mRawADCBufferHead = 0;


uint16_t ReadADC() 
{
//	ADCSRA |= (1 << ADEN); // Analog-Digital enable bit
	
	ADCSRA |= (1 << ADSC); // Start conversion
	while (ADCSRA & (1 << ADSC)); // wait until conversion is done
 /*
 //apparently when ready small values reading twice is better to allow "stabilization of the ADC"
 //read on adafruits
	_delay_ms(5);
	ADCSRA |= (1 << ADSC); // start single conversion
	while (ADCSRA & (1 << ADSC)) // wait until conversion is done
 
	ADCSRA &= ~(1<<ADEN); // shut down the ADC 
*/
	uint16_t vVal = ADCL;
	vVal = (ADCH << 8) + vVal;
	return vVal;
}

//Simple conversion by zone, no fading
//inline saves 50 byte ... 50byte for 2 jump and stack in/stack out ?
inline void Temp2RGB_Simple( uint8_t pTemp )	{
	mRGB[0] = 0;
	mRGB[1] = 0;
	mRGB[2] = 0;
			
	if (pTemp <= LIMIT_BLUEGREEN){
		//blue
		mRGB[2] = 255;
	}
	else {
		if (pTemp <= LIMIT_GREENRED){
			mRGB[1] = 255;
		}
		else {
			mRGB[0] = 255;
		}
	}
}

//Fading conversion
//inline saves 50 byte ... 50byte for 2 jump and stack in/stack out ?
inline void Temp2RGB_Fade( uint16_t pTemp )	{
			
	//Blue
	if (pTemp > LIMIT_BLUEGREEN+LIMIT_DELTA){
		mRGB[2] = 0;
	}
	else {
		if (pTemp < LIMIT_BLUEGREEN-LIMIT_DELTA){
			mRGB[2] = 255;
		}
		else {
			mRGB[2] = 255- ((255 * (pTemp - (LIMIT_BLUEGREEN - LIMIT_DELTA)) / (2 * LIMIT_DELTA)));
		}
	}
	
	
	//Green
	if (pTemp >= LIMIT_BLUEGREEN-LIMIT_DELTA && pTemp <= LIMIT_BLUEGREEN +LIMIT_DELTA){
		mRGB[1] = ((255) * (pTemp - (LIMIT_BLUEGREEN - LIMIT_DELTA)) / (2 * LIMIT_DELTA)) ;
	}
	else {
		if (pTemp > LIMIT_GREENRED-LIMIT_DELTA && pTemp <= LIMIT_GREENRED +LIMIT_DELTA){
			mRGB[1] = 255 - (((255) * (pTemp - (LIMIT_GREENRED - LIMIT_DELTA)) / (2 * LIMIT_DELTA)));
		}
		else {
			if (pTemp < LIMIT_BLUEGREEN-LIMIT_DELTA){
				mRGB[1] = 0;
			}
			else {
				if (pTemp > LIMIT_GREENRED + LIMIT_DELTA){
					mRGB[1] = 0;
				}
				else {
					
					mRGB[1] = 255;
				}
				
			}
		}
	}
	
	
	//Red
	if (pTemp >= LIMIT_GREENRED-LIMIT_DELTA && pTemp <= LIMIT_GREENRED +LIMIT_DELTA){
		mRGB[0] = ((255) * (pTemp - (LIMIT_GREENRED - LIMIT_DELTA)) / (2 * LIMIT_DELTA));
	}
	else {
		if (pTemp < LIMIT_GREENRED-LIMIT_DELTA){
			mRGB[0] = 0;
		}
		else {
			mRGB[0] = 255;
		}
	}
	
	
}

int main(void)
{
	mRGB[0] = 0;
	mRGB[1] = 0;
	mRGB[2] = 0;

	
	//Timer full speed !
	CLKPR = (1<<CLKPCE);
	CLKPR = 0; // Divide by 1 
	
	//port B pins 0-5 go OUTPUT
    DDRB = 0xFF;
	
	
	//------------START ADC INIT --------------------------
	 ADCSRA |= (1 << ADEN) | // Analog-Digital enable bit
 (1 << ADPS1)| // set prescaler to 8 (clock / 8)
 (1 << ADPS0); // set prescaler to 8 (clock / 8)
 ;
 ADMUX |= 
	(0 << ADLAR) //store the result left adjusted, meaning must read ADCL (LSB) then ADCH (MSB) 
	| (1 << MUX1); // Choose AD input AD2 (BP 4)
	//------------END ADC INIT --------------------------
	
	
	uint8_t vADCCheckRound = 0;
	uint8_t vRGBCurrentRound = 0;
    while(1)
    {
		
		if(vADCCheckRound == 0){
			uint16_t vADCValue = ReadADC();
			
			//store
			mRawADCBuffer[mRawADCBufferHead] = vADCValue;
			mRawADCBufferHead = (mRawADCBufferHead + 1) % READING_BUFFER;
			
			//average
			uint16_t vAvg = 0;
			for (int i =0; i < READING_BUFFER;i++){
				vAvg += mRawADCBuffer[i];
			}
			//overwrite (optimized, we know there's 4 items so >>2)
			//vADCValue = vAvg / READING_BUFFER;
			vADCValue = (vAvg >> 2);
			
			//value read is [0;1024]. To get a temp you *5 and divide by 1024 (=1000)
			//But then you've got a float value [0;5] (remember 1 degree C = 0.01v)
			//So you would multiply by 100 to get a temp (0.16V -> 16 degree)
			//Working just with integer here I shortcut the / 1000 * 100 -> /10
			//Then we have vADCValue * 5 /10 -> vADCValue *1/2 -> vADCValue >> 1
			uint16_t vTemp = vADCValue >> 1;
			
			Temp2RGB_Fade(vTemp);

		}
		
		
		
        //todo : put this in an interrupt
		if (vRGBCurrentRound == 0){
			/*
			//make pins B0-2 high
			PORTB = 
				(1 << PORTB0) 
				| (1 << PORTB1)
				| (1 << PORTB2)
				;
			*/
			
			PORTB = 0x00;
			if (mRGB[0] > 0)
				PORTB |= (1 << PORTB0);
			if (mRGB[1] > 0)
				PORTB |= (1 << PORTB1);
			if (mRGB[2] > 0)
				PORTB |= (1 << PORTB2);
				
		}
		

		if (vRGBCurrentRound == mRGB[0] && vRGBCurrentRound != 255){
			PORTB &= ~(1 << PORTB0);
		}

	
		if (vRGBCurrentRound == mRGB[1] && vRGBCurrentRound != 255){
			PORTB &= ~(1 << PORTB1);
		}
		

		if (vRGBCurrentRound == mRGB[2] && vRGBCurrentRound != 255){
			PORTB &= ~(1 << PORTB2);
		}
		
		//and the POV
		_delay_us(50);
		vRGBCurrentRound++;
		
		vADCCheckRound++;
    }
	
	
}