#include "TouchSlideExt.h"


//
// Screensaver for Touchslide
//
//
//


#define BEES_COUNT 16
#define BEE_RAYON 3

typedef struct {
	int x;
	int y;
} Bee;


Bee mBees[BEES_COUNT];


int newRandBees(int pV, int pMin, int pMax){
	int vRand = random(0, BEE_RAYON*2+1) - BEE_RAYON;
	if (vRand + (int)pV < pMin) return pMin;
	else
	if (vRand + (int)pV > pMax) return pMax;
	else
	return (int)pV + vRand;
}

void drawBee(Bee& pB){
	//diag
	//line(pB.x -BEE_RAYON, pB.y -BEE_RAYON, pB.x +BEE_RAYON, pB.y +BEE_RAYON);

	line(pB.x -BEE_RAYON, pB.y, pB.x +BEE_RAYON, pB.y);
	line(pB.x, pB.y -BEE_RAYON, pB.x, pB.y +BEE_RAYON);
}

//Start drawing the bees
void beesFlight(bool pCheckSerial){
	//while not touched, animate...
	while(!gettouch() && (!pCheckSerial || Serial.available() == 0)){
		for(int i=0; i < BEES_COUNT; i++){
			mBees[i].x = newRandBees(mBees[i].x, BEE_RAYON, 320-BEE_RAYON);
			mBees[i].y = newRandBees(mBees[i].y, BEE_RAYON, 240-BEE_RAYON); 
			
			drawBee(mBees[i]);
		}

		delay(50);
	}
}

//Initialize the bees struct
void initBees(){
	//turn off contrast
	setbrightness(0);
	//paint it black
	stroke(0);

	for(int i=0; i < BEES_COUNT; i++){
		mBees[i].x = 160; // =320/2
		mBees[i].y = 120; 
	}
}



