#include "TouchSlideExt.h"


RECT mNextAutoposition; //NEXT position where to write button if autoPosition
ARDTButton mButtons[BUTTONS_COUNT]; //all buttons
ARDTButton *mButtonDisplayLabel; //the button that will be the display for text
byte mButtonsCurrentCount = 0; //number of buttons on the screen currently

ARDTButton* getAllButtons (){ return mButtons; }
ARDTButton* getDisplayLabel (){ return mButtonDisplayLabel; }
ARDTButton* setDisplayLabel (ARDTButton* pB){ mButtonDisplayLabel = pB; return getDisplayLabel(); }
byte getAllButtonsCount (){ return mButtonsCurrentCount; }
byte setAllButtonsCount (byte pCount){ mButtonsCurrentCount = pCount; }


//highlights a image button
void highlightARDTButtonImage (ARDTButton& pButton){
	noFill();
	stroke(200);
	rect(pButton.rect.left, pButton.rect.top, BUTTON_SPACE, BUTTON_SPACE);
	stroke(150);
	rect(pButton.rect.left+1, pButton.rect.top+1, BUTTON_SPACE-2, BUTTON_SPACE-2);
	stroke(100);
	rect(pButton.rect.left+2, pButton.rect.top+2, BUTTON_SPACE-4, BUTTON_SPACE-4);
	delay(100);
	stroke(0);
	rect(pButton.rect.left, pButton.rect.top, BUTTON_SPACE, BUTTON_SPACE);
	rect(pButton.rect.left+1, pButton.rect.top+1, BUTTON_SPACE-2, BUTTON_SPACE-2);
	rect(pButton.rect.left+2, pButton.rect.top+2, BUTTON_SPACE-4, BUTTON_SPACE-4);
}

//updates the latest position, by adding known size
void UpdateNextAutoPosition(){
	mNextAutoposition.left += BUTTON_SPACE;

	if (SCREEN_WIDTH - mNextAutoposition.left < BUTTON_SPACE){
		//not enough space on that line, move next line
		mNextAutoposition.left = BUTTON_AUTOPOSITION_START_LEFT;
		mNextAutoposition.top += BUTTON_SPACE;
	}
}

//Draws a button with an image
void drawARDTButtonImage (ARDTButton& pButton){
	////+6 to center : space is 60 pixel, image is 48 so remaing 12/2

	int vX = 0;
	int vY = 0;

	if ((pButton.attributes & BUTTON_ATTRIBUTE_AUTOPOSITION)> 0){
		//calculate where to put it
		vX = mNextAutoposition.left;
		vY = mNextAutoposition.top;
		
		//position has been determined, so override memorized rectangle for touch detection later on
		pButton.rect.left = vX;
		pButton.rect.top = vY;
		pButton.rect.right = vX + BUTTON_SPACE;
		pButton.rect.bottom = vY + BUTTON_SPACE;
		
		UpdateNextAutoPosition();
	}
	else {
		vX = pButton.rect.left;
		vY = pButton.rect.top;
	}

	if (pButton.imageIndex != 254){
		imageByIndex (pButton.imageIndex, vX+6, vY+6);
	}
	else {
		image (loadImage(pButton.imageName), vX+6, vY+6);  
	}
}

//Draws a button
void drawARDTButton (ARDTButton& pButton){
	if (pButton.imageIndex < 255){
		drawARDTButtonImage (pButton);
		return;
	}

	byte r, g, b;
	r = (byte)((pButton.bgcolor & 0xFF0000) >> 16);
	g = (byte)((pButton.bgcolor & 0x00FF00) >> 8);
	b = (byte)((pButton.bgcolor & 0x0000FF));
	fill(r,g,b); //fill 

	//button DRAWN is smaller than its zone
	RECT vDrawRect;
	byte vPadding = ((pButton.attributes & BUTTON_ATTRIBUTE_NOPADDING)> 0 ? 0 : 6);
	vDrawRect.top = pButton.rect.top + vPadding;
	vDrawRect.bottom = pButton.rect.bottom - vPadding;
	vDrawRect.left = pButton.rect.left + vPadding;
	vDrawRect.right = pButton.rect.right - vPadding;
	
	if ((pButton.attributes & BUTTON_ATTRIBUTE_BGGRADIENT)> 0){	
		//GRADIENT FILL
		noStroke();
		for (int i=vDrawRect.top; i < vDrawRect.bottom; i += BTN_FILL_STEP){
			byte vHeight = BTN_FILL_STEP;
			if (i + vHeight > vDrawRect.bottom) vHeight = vDrawRect.bottom - i;
			rect(vDrawRect.left,i,vDrawRect.right - vDrawRect.left,vHeight); 
			if (r > BTN_FILL_STEP) r -= BTN_FILL_STEP;
			if (g > BTN_FILL_STEP) g -= BTN_FILL_STEP;
			if (b > BTN_FILL_STEP) b -= BTN_FILL_STEP;
			fill(r,g,b); //fill 
		}
	}
	else {
		//UNIFORM PAINT
		rect(vDrawRect.left,vDrawRect.top,vDrawRect.right - vDrawRect.left,vDrawRect.bottom - vDrawRect.top); 	
	}

	if ((pButton.attributes & BUTTON_ATTRIBUTE_HASBORDER)> 0){
		//finally draw border
		stroke(180,180,180);
		noFill();
		rect(vDrawRect.left,vDrawRect.top,vDrawRect.right - vDrawRect.left,vDrawRect.bottom - vDrawRect.top); 
	}

	//last draw the text
	r = (pButton.forecolor & 0x00FF0000) >> 16;
	g = (pButton.forecolor & 0x0000FF00) >> 8;
	b = (pButton.forecolor & 0x000000FF);
	stroke(r,g,b);
	noFill();
	text(pButton.label,vDrawRect.left+4, vDrawRect.top + ((vDrawRect.bottom - vDrawRect.top) /2)); //print the character string 
}




void drawAllButtons(){
	//set initial position for the buttons (only top-left is important)
	mNextAutoposition.top = BUTTON_AUTOPOSITION_START_TOP;
	mNextAutoposition.left = BUTTON_AUTOPOSITION_START_LEFT;
	mNextAutoposition.right = 0;
	mNextAutoposition.bottom = 0;

	//draws all the buttons
	for (int i = 0; i < mButtonsCurrentCount; i++){
		drawARDTButton(mButtons[i]);
	}
}

//Shows a text in the text zone
void showMessage(char* pS){
	showMessage(pS, (mButtonDisplayLabel->rect.bottom - mButtonDisplayLabel->rect.top) / 2);
}

//Shows a text in the text zone
void showMessage(char* pS, byte pFontSize){
	if (mButtonDisplayLabel == 0) return;
	
	//Refresh the display of the label display button
	drawARDTButton(*mButtonDisplayLabel);
	
	HersheyDrawCString(
	kHerheyFont_Futural, 
	mButtonDisplayLabel->rect.left+5, 
	mButtonDisplayLabel->rect.top + ((mButtonDisplayLabel->rect.bottom - mButtonDisplayLabel->rect.top) /2), 
	pS, 
	pFontSize, 
	0, 
	1); 
}

//Returns the button that is at a given coordinate
ARDTButton* getButtonAt (Point p){
	//search among all buttons if one was pressed
	for (int i = 0; i < mButtonsCurrentCount; i++){
		if (PtInRect(p, &(mButtons[i].rect))){
			return &mButtons[i];
		}
	}

	return 0;
}