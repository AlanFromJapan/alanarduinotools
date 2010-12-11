#include "TouchSlideExt.h"

//
// Screensaver for Touchslide
//
//
//
#include <WProgram.h>


#define BOX_COUNT 12
#define BOX_SIZE 5
#define BG_GREY 128

//#define ERASE_BG

typedef struct {
	int x;
	int y;

	int deltaX;
	int deltaY;
} Bbox;


Bbox mBouncingBoxes[BOX_COUNT];

void moveBox(Bbox& pB){
	pB.x += pB.deltaX; 
	pB.y += pB.deltaY;

	if (pB.x - BOX_SIZE <= 0 || pB.x + BOX_SIZE >= 320){
		//bounce
		pB.deltaX = -pB.deltaX;
	}

	if (pB.y - BOX_SIZE <= 0 || pB.y + BOX_SIZE >= 240){
		//bounce
		pB.deltaY = -pB.deltaY;
	}

}

void drawBox(Bbox& pB){
	rect(pB.x - BOX_SIZE, pB.y - BOX_SIZE, 2*BOX_SIZE, 2*BOX_SIZE);
}

//Start drawing the boxes
void boxesFlight(bool pCheckSerial){
	//while not touched, animate...
	while(!gettouch() && (!pCheckSerial || Serial.available() == 0)){
		for(int i=0; i < BOX_COUNT; i++){
#ifdef ERASE_BG
			fill(BG_GREY);
			drawBox(mBouncingBoxes[i]);
#endif

			moveBox(mBouncingBoxes[i]);
			
			fill(0);
			drawBox(mBouncingBoxes[i]);
		}

		delay(50);
	}
}

//Initialize the bees struct
void initBoxes(){
	//turn off contrast
	setbrightness(0);
	//paint it black
	noStroke();
	//fill black
	fill(0);

	for(int i=0; i < BOX_COUNT; i++){
		mBouncingBoxes[i].x = random(10, 300);
		mBouncingBoxes[i].y = random(10, 200);

		/* 1pixel move
	mBouncingBoxes[i].deltaX = -1 + random(0, 2) * 2;
	mBouncingBoxes[i].deltaY = -1 + random(0, 2) * 2;
	*/    
		mBouncingBoxes[i].deltaX = -3 + random(0, 7);
		mBouncingBoxes[i].deltaY = -3 + random(0, 7);    
	}
}
