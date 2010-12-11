/*
* TouchSlideExt 
* Extended library for TouchSlide
* by Alain VIZZINI (may 2009)
* Free licensing - redistribute and use freely
*/

#ifndef TouchSlideExt_Lib_h
#define TouchSlideExt_Lib_h


#include "QuickDraw.h"

//fonts
#define _ENABLE_HERSHEY_FONTS_
#include "HersheyFonts.h" 

//#define TouchSlideExt_Lib_DEBUGMODE

#include <WProgram.h>

///////////////////////////////////////////////////////////////////////////////////
// Constants
//width of the space containing an image button
//NB: image MUST be 48x48
#define BUTTON_SPACE 60
#define BUTTON_AUTOPOSITION_START_TOP 5
#define BUTTON_AUTOPOSITION_START_LEFT 10

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define BG_FILL_STEP 12
#define BTN_FILL_STEP 8
//#define BG_GRADIENT 

///////////////////////////////////////////////////////////////////////////////////
// Structures
//ARDTButton struct definition
//uses char* to save space : no need to copy the text here, referencing it in the string zone is sufficient (no change)
typedef struct {
  byte attributes; //see under
  RECT rect; //wether fixed or not, this is where the button has been drawn
  char* label; //label of the button (displayed in info bar)
  char code; //code that will be sent to the PC
  long bgcolor; //background color (coded on a long)
  long forecolor; //text color (coded on a long)
  byte imageIndex; //255: it's a text button, 254 : image by name, otherwise the index of the image
  char* imageName; //if it is a image button by name, the name of the image
} ARDTButton;

#define BUTTON_ATTRIBUTE_AUTOPOSITION 1
#define BUTTON_ATTRIBUTE_HASBORDER 2
#define BUTTON_ATTRIBUTE_HASCONFIRMATION 4
#define BUTTON_ATTRIBUTE_BGGRADIENT 8
#define BUTTON_ATTRIBUTE_NOPADDING 16 //dont make smaller to keep some space around the button

//Buttons management
#define BUTTONS_COUNT 20 //max number of buttons allowed on one screen


//Panels (set of buttons)
#define PANEL_DEFAULT 0
#define PANEL_YESNO 1
#define PANEL_CONFIG 2
#define PANEL_MEDIA 3 
//byte mCurrentPanel = 0;



///////////////////////////////////////////////////////////////////////////////////
// Signatures declaration
//
//Displays all the known images
void ListKnownImages();
void imageByIndex(short pIndex, int xLoc, int yLoc);
void showNextImageByIndex(int pX, int pY);
///////////////////////////////////////////////////////////////////////////////////
//ScreenSavers

//Start drawing the boxes
void boxesFlight(bool pCheckSerial);
//Initialize the bees struct
void initBoxes();

//Start drawing the bees
void beesFlight(bool pCheckSerial);
//Initialize the bees struct
void initBees();

///////////////////////////////////////////////////////////////////////////////////
// Buttons
ARDTButton* getAllButtons ();
ARDTButton* getDisplayLabel ();
ARDTButton* setDisplayLabel (ARDTButton* pB);
byte getAllButtonsCount ();
byte setAllButtonsCount (byte pCount);

//highlights a image button
void highlightARDTButtonImage (ARDTButton& pButton);
//updates the latest position, by adding known size
void UpdateNextAutoPosition();
//Draws a button with an image
void drawARDTButtonImage (ARDTButton& pButton);
//Draws a button
void drawARDTButton (ARDTButton& pButton);

void drawAllButtons();
//Shows a text in the text zone
void showMessage(char* pS);
void showMessage(char* pS, byte pFontSize);

//Returns the button that is at a given coordinate
ARDTButton* getButtonAt (Point p);

///////////////////////////////////////////////////////////////////////////////////
// Panels
//moves to a defined panel
void gotoPanel(byte pPanelId);
//moves to a defined panel
bool showConfirmMessage(char* pMessage);
//gets the id of the current panel
byte getCurrentPanel() ;

void setupButtons();
#endif /* TouchSlideExt_Lib_h */