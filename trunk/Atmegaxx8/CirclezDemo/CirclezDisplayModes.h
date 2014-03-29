/*
 * CirclezDisplayModes.h
 *
 * Created: 2014/03/29 23:55:26
 *  Author: Alan
 */ 


#ifndef CIRCLEZDISPLAYMODES_H_
#define CIRCLEZDISPLAYMODES_H_


#define LED_COUNT 24
uint8_t mLedVal[LED_COUNT];
uint8_t mLedVal2[LED_COUNT];



#define MODE_ONE_STRIP			0
#define MODE_TWO_STRIP_COLLIDE	1
#define MODE_TWO_STRIP_CHASING	2
#define MODE_3_STRIP_CHASING	3
#define MODE_N_STRIP_CHASING	4
#define MODE_N_STRIP_CHASING2	5
uint8_t mCurrentMode = MODE_ONE_STRIP;



//puts all values of the array to 0
void zeroArray (uint8_t* pArray){
	for (uint8_t i = 0; i < LED_COUNT; i++){
		pArray[i] = 0;
	}
}

void rotateArray(uint8_t* pArray, int8_t pDirection){
	if (pDirection > 0){
		uint8_t vTemp = pArray[LED_COUNT-1];
		uint8_t i = LED_COUNT-2;
		do
		{
			pArray[i+1] = pArray[i];
			i--;
		} while (i > 0);
		pArray[1] = pArray[0];
		pArray[0] = vTemp;
	}
	else{
		uint8_t vTemp = pArray[0];
		uint8_t i = 1;
		do
		{
			pArray[i-1] = pArray[i];
			i++;
		} while (i < LED_COUNT);
		pArray[LED_COUNT-1] = vTemp;
	}
}


//Updates the data of the array
inline void updateArrays(){
	switch(mCurrentMode){
		case MODE_ONE_STRIP:
		case MODE_3_STRIP_CHASING:
		case MODE_N_STRIP_CHASING:
		case MODE_N_STRIP_CHASING2:
		rotateArray(mLedVal, -1);
		break;
		case MODE_TWO_STRIP_COLLIDE:
		rotateArray(mLedVal, -1);
		rotateArray(mLedVal2, 1);
		break;
		case MODE_TWO_STRIP_CHASING:
		rotateArray(mLedVal, -1);
		rotateArray(mLedVal2, -1);
		break;
	}
}

//Inits the data of the array
inline void initArrays(){
	
	zeroArray(mLedVal);
	zeroArray(mLedVal2);
	
	switch(mCurrentMode){
		case MODE_ONE_STRIP:
		mLedVal[0] = 255;
		mLedVal[1] = 192;
		mLedVal[2] = 128;
		mLedVal[3] = 64;
		mLedVal[4] = 32;
		mLedVal[5] = 16;
		mLedVal[6] = 8;
		break;
		
		case MODE_TWO_STRIP_COLLIDE:
		mLedVal[0] = 255;
		mLedVal[1] = 192;
		mLedVal[2] = 128;
		mLedVal[3] = 64;
		mLedVal[4] = 32;
		mLedVal[5] = 16;
		mLedVal[6] = 8;
		
		mLedVal2[6] = 255;
		mLedVal2[5] = 192;
		mLedVal2[4] = 128;
		mLedVal2[3] = 64;
		mLedVal2[2] = 32;
		mLedVal2[1] = 16;
		mLedVal2[0] = 8;
		break;
		
		case MODE_TWO_STRIP_CHASING:
		mLedVal[0] = 255;
		mLedVal[1] = 192;
		mLedVal[2] = 128;
		mLedVal[3] = 64;
		mLedVal[4] = 32;
		mLedVal[5] = 16;
		mLedVal[6] = 8;

		mLedVal2[12] = 255;
		mLedVal2[13] = 192;
		mLedVal2[14] = 128;
		mLedVal2[15] = 64;
		mLedVal2[16] = 32;
		mLedVal2[17] = 16;
		mLedVal2[18] = 8;
		break;
		
		case MODE_3_STRIP_CHASING:
		mLedVal[0] = 32;
		mLedVal[1] = 192;
		mLedVal[2] = 192;
		mLedVal[3] = 32;

		mLedVal[8] = 32;
		mLedVal[9] = 192;
		mLedVal[10] = 192;
		mLedVal[11] = 32;

		mLedVal[16] = 32;
		mLedVal[17] = 192;
		mLedVal[18] = 192;
		mLedVal[19] = 32;
		
		break;
		
		case MODE_N_STRIP_CHASING:
		for (uint8_t i = 0; i < LED_COUNT; ){
			mLedVal[i] = 32;
			mLedVal[i+1] = 192;
			mLedVal[i+2] = 32;
			i+= 3;
		}		
		
		break;
		
		case MODE_N_STRIP_CHASING2:
		for (uint8_t i = 0; i < LED_COUNT; ){
			mLedVal[i] = 16;
			mLedVal[i+1] = 192;
			mLedVal[i+2] = 16;
			i+= 6;
		}
		
		break;
	}
}


#endif /* CIRCLEZDISPLAYMODES_H_ */