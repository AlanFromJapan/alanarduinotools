#include "TouchSlideExt.h"



byte mCurrentPanel = 0;
byte getCurrentPanel() { return mCurrentPanel; }


//The default set of buttons
void setupButtons_PANEL_DEFAULT(){
	byte i = 0;
	ARDTButton* vAllButtons = getAllButtons();

	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_AUTOPOSITION;
	vAllButtons[i].label ="calc";
	vAllButtons[i].bgcolor = 0x00FFFFC0;
	vAllButtons[i].forecolor = 0x00000000;
	vAllButtons[i].code = 'X';
	vAllButtons[i].imageIndex = 254; //by name
	vAllButtons[i].imageName = "calc.bmp"; //strcpy(&(vAllButtons[i].imageName[0]), "console.bmp");
	i++;

	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_AUTOPOSITION;
	vAllButtons[i].label ="run cmd";
	vAllButtons[i].bgcolor = 0x00A0A0FF;
	vAllButtons[i].forecolor = 0x00000000;
	vAllButtons[i].code = 'C';
	vAllButtons[i].imageIndex = 254; //by name
	vAllButtons[i].imageName = "console.bmp"; //strcpy(&(vAllButtons[i].imageName[0]), "console.bmp");
	i++;

	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_AUTOPOSITION;
	vAllButtons[i].label ="character map";
	vAllButtons[i].bgcolor = 0x0080B080;
	vAllButtons[i].forecolor = 0x00000000;
	vAllButtons[i].code = 'Z';
	vAllButtons[i].imageIndex = 254; //by name
	vAllButtons[i].imageName = "char.bmp";
	i++;

	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_AUTOPOSITION | BUTTON_ATTRIBUTE_HASCONFIRMATION;
	vAllButtons[i].label ="The Gimp!";
	vAllButtons[i].bgcolor = 0x0080B080;
	vAllButtons[i].forecolor = 0x00000000;
	vAllButtons[i].code = 'G';
	vAllButtons[i].imageIndex = 254; //by name
	vAllButtons[i].imageName = "gimp.bmp";
	i++;

	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_AUTOPOSITION;
	vAllButtons[i].label ="logo";
	vAllButtons[i].bgcolor = 0x0080B080;
	vAllButtons[i].forecolor = 0x00000000;
	vAllButtons[i].code = 'L';
	vAllButtons[i].imageIndex = 254; //by name
	vAllButtons[i].imageName = "restart.bmp";
	i++;

	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_AUTOPOSITION;
	vAllButtons[i].label ="note";
	vAllButtons[i].bgcolor = 0x00FFFFC0;
	vAllButtons[i].forecolor = 0x00000000;
	vAllButtons[i].code = 'N';
	vAllButtons[i].imageIndex = 254; //by name
	vAllButtons[i].imageName = "notepad.bmp";
	i++;

	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_AUTOPOSITION | BUTTON_ATTRIBUTE_HASCONFIRMATION;
	vAllButtons[i].label ="eaBook -= PROD =-";
	vAllButtons[i].bgcolor = 0x00FFFFC0;
	vAllButtons[i].forecolor = 0x00000000;
	vAllButtons[i].code = 'E';
	vAllButtons[i].imageIndex = 254; //by name
	vAllButtons[i].imageName = "eaBprod.bmp";
	i++;

	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_AUTOPOSITION | BUTTON_ATTRIBUTE_HASCONFIRMATION;
	vAllButtons[i].label ="eaBook -= PRE-PROD =-";
	vAllButtons[i].bgcolor = 0x00FFFFC0;
	vAllButtons[i].forecolor = 0x00000000;
	vAllButtons[i].code = 'F';
	vAllButtons[i].imageIndex = 254; //by name
	vAllButtons[i].imageName = "eaBpre.bmp";
	i++;

	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_AUTOPOSITION;
	vAllButtons[i].label ="Music";
	vAllButtons[i].bgcolor = 0x00FFFFC0;
	vAllButtons[i].forecolor = 0x00000000;
	vAllButtons[i].code = 'M';
	vAllButtons[i].imageIndex = 254; //by name
	vAllButtons[i].imageName = "music1.bmp";
	i++;

	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_AUTOPOSITION;
	vAllButtons[i].label ="Config";
	vAllButtons[i].bgcolor = 0x00FFFFC0;
	vAllButtons[i].forecolor = 0x00000000;
	vAllButtons[i].code = 'K';
	vAllButtons[i].imageIndex = 254; //by name
	vAllButtons[i].imageName = "wand.bmp";
	i++;

	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_AUTOPOSITION;
	vAllButtons[i].label ="-tba-";
	vAllButtons[i].bgcolor = 0x00FFFFC0;
	vAllButtons[i].forecolor = 0x00000000;
	vAllButtons[i].code = '5';
	vAllButtons[i].imageIndex = 254; //by name
	vAllButtons[i].imageName = "ff5a.bmp";
	i++;


	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_AUTOPOSITION;
	vAllButtons[i].label ="Cobra";
	vAllButtons[i].bgcolor = 0x00FFFFC0;
	vAllButtons[i].forecolor = 0x00000000;
	vAllButtons[i].code = 'A';
	vAllButtons[i].imageIndex = 254; //by name
	vAllButtons[i].imageName = "cobra48.bmp";
	i++;


	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_AUTOPOSITION;
	vAllButtons[i].label ="Calendar";
	vAllButtons[i].bgcolor = 0x00FFFFC0;
	vAllButtons[i].forecolor = 0x00000000;
	vAllButtons[i].code = 'c';
	vAllButtons[i].imageIndex = 254; //by name
	vAllButtons[i].imageName = "cal.bmp";
	i++;
	
	
	/////////////////////////////////////////////////////////
	// Fized size buttons
	/////////////////////////////////////////////////////////

	/*
vAllButtons[i].autoPosition = false;
vAllButtons[i].rect.top = 180;
vAllButtons[i].rect.left = 20;
vAllButtons[i].rect.right = 300;
vAllButtons[i].rect.bottom = 210;
vAllButtons[i].label ="Brightness selector";
vAllButtons[i].bgcolor = 0x00A0A0A0;
vAllButtons[i].forecolor = 0x00000000;
vAllButtons[i].code = 'B';
vAllButtons[i].imageIndex = 255;
i++;
*/

	//SPECIAL : that button is the display label ! Save it!
	setDisplayLabel(&vAllButtons[i]);
	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_HASBORDER | BUTTON_ATTRIBUTE_NOPADDING;
	vAllButtons[i].rect.top = 210;
	vAllButtons[i].rect.left = 10;
	vAllButtons[i].rect.right = 310;
	vAllButtons[i].rect.bottom = 238;
	vAllButtons[i].label ="";
	vAllButtons[i].bgcolor = 0x00B0B0B0; //0x0000A070;
	vAllButtons[i].forecolor = 0x00000000;
	vAllButtons[i].code = 'T';
	vAllButtons[i].imageIndex = 255;
	i++;

	//total number of buttons
	setAllButtonsCount (i);
}

//Buttons for a YES / NO panel
void setupButtons_PANEL_YESNO(){
	byte i = 0;
	ARDTButton* vAllButtons = getAllButtons();

	//SPECIAL : that button is the display label ! Save it!
	setDisplayLabel(&vAllButtons[i]);
	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_NOPADDING;
	vAllButtons[i].rect.top = 0;
	vAllButtons[i].rect.left = 0;
	vAllButtons[i].rect.right = 320;
	vAllButtons[i].rect.bottom = 60;
	vAllButtons[i].label ="";
	vAllButtons[i].bgcolor = 0; 
	vAllButtons[i].forecolor = 0x00FFFFFF;
	vAllButtons[i].code = 'T';
	vAllButtons[i].imageIndex = 255; //text button
	i++;


	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_HASBORDER;
	vAllButtons[i].rect.top = 80;
	vAllButtons[i].rect.left = 20;
	vAllButtons[i].rect.right = 140;
	vAllButtons[i].rect.bottom = 160;
	vAllButtons[i].label ="YES";
	vAllButtons[i].bgcolor = 0x0000FF00; 
	vAllButtons[i].forecolor = 0x00FFFFFF;
	vAllButtons[i].code = 'Y';
	vAllButtons[i].imageIndex = 255; //text button
	i++;

	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_HASBORDER;
	vAllButtons[i].rect.top = 80;
	vAllButtons[i].rect.left = 180;
	vAllButtons[i].rect.right = 310;
	vAllButtons[i].rect.bottom = 160;
	vAllButtons[i].label ="NO";
	vAllButtons[i].bgcolor = 0x00FF0000; 
	vAllButtons[i].forecolor = 0x00FFFFFF;
	vAllButtons[i].code = 'N';
	vAllButtons[i].imageIndex = 255; //text button
	i++;

	//total number of buttons
	setAllButtonsCount (i);
}

//The default set of buttons
void setupButtons_PANEL_CONFIG(){
	byte i = 0;
	ARDTButton* vAllButtons = getAllButtons();
	setDisplayLabel(0);
	
	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_AUTOPOSITION ;
	vAllButtons[i].label ="logo";
	vAllButtons[i].bgcolor = 0x0080B080;
	vAllButtons[i].forecolor = 0x00000000;
	vAllButtons[i].code = 'L';
	vAllButtons[i].imageIndex = 254; //by name
	vAllButtons[i].imageName = "restart.bmp";
	i++;
	
	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_AUTOPOSITION| BUTTON_ATTRIBUTE_HASBORDER | BUTTON_ATTRIBUTE_BGGRADIENT;
	vAllButtons[i].label ="List img";
	vAllButtons[i].bgcolor = 0x0080B080;
	vAllButtons[i].forecolor = 0x00000000;
	vAllButtons[i].code = 'i';
	vAllButtons[i].imageIndex = 255; //text
	//vAllButtons[i].imageName = "restart.bmp";
	i++;
	
	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_AUTOPOSITION| BUTTON_ATTRIBUTE_HASBORDER;
	vAllButtons[i].label ="Next img";
	vAllButtons[i].bgcolor = 0x007090F0;
	vAllButtons[i].forecolor = 0x00FFFFFF;
	vAllButtons[i].code = '\\';
	vAllButtons[i].imageIndex = 255; //text
	//vAllButtons[i].imageName = "restart.bmp";
	i++;
	
	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_AUTOPOSITION| BUTTON_ATTRIBUTE_HASBORDER  | BUTTON_ATTRIBUTE_HASCONFIRMATION;
	vAllButtons[i].label ="Upload";
	vAllButtons[i].bgcolor = 0x00C00000;
	vAllButtons[i].forecolor = 0x00FFFFFF;
	vAllButtons[i].code = '^';
	vAllButtons[i].imageIndex = 255; //text
	//vAllButtons[i].imageName = "restart.bmp";
	i++;
	
	//total number of buttons
	setAllButtonsCount (i);
}

//The default set of buttons
void setupButtons_PANEL_MEDIA(){
	byte i = 0;
	ARDTButton* vAllButtons = getAllButtons();
	setDisplayLabel(0);
	
	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_AUTOPOSITION;
	vAllButtons[i].label ="logo";
	vAllButtons[i].bgcolor = 0x0080B080;
	vAllButtons[i].forecolor = 0x00000000;
	vAllButtons[i].code = 'L';
	vAllButtons[i].imageIndex = 254; //by name
	vAllButtons[i].imageName = "restart.bmp";
	i++;
	
	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_HASBORDER;
	vAllButtons[i].rect.top = 80;
	vAllButtons[i].rect.left = 10;
	vAllButtons[i].rect.right = 70;
	vAllButtons[i].rect.bottom = 140;
	vAllButtons[i].label ="  <<";
	vAllButtons[i].bgcolor = 0x007090F0;
	vAllButtons[i].forecolor = 0x00FFFFFF;
	vAllButtons[i].code = '<';
	vAllButtons[i].imageIndex = 254; //by name
	vAllButtons[i].imageName = "wmpPre.bmp";
	i++;
	
	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_HASBORDER;
	vAllButtons[i].rect.top = 80;
	vAllButtons[i].rect.left = 70;
	vAllButtons[i].rect.right = 130;
	vAllButtons[i].rect.bottom = 140;
	vAllButtons[i].label ="  | |";
	vAllButtons[i].bgcolor = 0x007090F0;
	vAllButtons[i].forecolor = 0x00FFFFFF;
	vAllButtons[i].code = '|';
	vAllButtons[i].imageIndex = 254; //by name
	vAllButtons[i].imageName = "wmpPaus.bmp";
	i++;
	
	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_HASBORDER;
	vAllButtons[i].rect.top = 80;
	vAllButtons[i].rect.left = 130;
	vAllButtons[i].rect.right = 190;
	vAllButtons[i].rect.bottom = 140;
	vAllButtons[i].label ="  >>";
	vAllButtons[i].bgcolor = 0x007090F0;
	vAllButtons[i].forecolor = 0x00FFFFFF;
	vAllButtons[i].code = '>';
	vAllButtons[i].imageIndex = 254; //by name
	vAllButtons[i].imageName = "wmpNex.bmp";
	i++;
		
	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_HASBORDER;
	vAllButtons[i].rect.top = 140;
	vAllButtons[i].rect.left = 70;
	vAllButtons[i].rect.right = 130;
	vAllButtons[i].rect.bottom = 200;
	vAllButtons[i].label ="  Vol-";
	vAllButtons[i].bgcolor = 0x0070F090;
	vAllButtons[i].forecolor = 0x00FFFFFF;
	vAllButtons[i].code = 'v';
	vAllButtons[i].imageIndex = 254; //by name
	vAllButtons[i].imageName = "wmpVdo.bmp";
	i++;
		
	vAllButtons[i].attributes = BUTTON_ATTRIBUTE_HASBORDER;
	vAllButtons[i].rect.top = 140;
	vAllButtons[i].rect.left = 130;
	vAllButtons[i].rect.right = 190;
	vAllButtons[i].rect.bottom = 200;
	vAllButtons[i].label ="  Vol+";
	vAllButtons[i].bgcolor = 0x0070F090;
	vAllButtons[i].forecolor = 0x00FFFFFF;
	vAllButtons[i].code = 'V';
	vAllButtons[i].imageIndex = 254; //by name
	vAllButtons[i].imageName = "wmpVup.bmp";
	i++;

	//total number of buttons
	setAllButtonsCount (i);
}

//creates the buttons for the current panel
void setupButtons(){
	switch (mCurrentPanel){
	case PANEL_DEFAULT:
		setupButtons_PANEL_DEFAULT();
		break;
	case PANEL_YESNO:
		setupButtons_PANEL_YESNO();
		break;
	case PANEL_MEDIA:
		setupButtons_PANEL_MEDIA();
		break;
	case PANEL_CONFIG:
		setupButtons_PANEL_CONFIG();
		break;
	}
}

//moves to a defined panel
void gotoPanel(byte pPanelId){
	fill(0);
	background(0,0,0);// set the screen black
	
	mCurrentPanel = pPanelId;
	
	setupButtons();
	
	drawAllButtons();
}


//Temporarily moves to the yes-no panel and returns the click on button yes or no
bool showConfirmMessage(char* pMessage){
	byte vPreviousPanel = mCurrentPanel;
	gotoPanel(PANEL_YESNO);
	showMessage(pMessage, 10);
	
	bool vResult = false;
	
	//wait for input
	bool vChoosed = false;
	while (!vChoosed){
		if (gettouch()){
			Point p;
			p.v = mouseY;
			p.h = mouseX;
			
			ARDTButton* vB = getButtonAt (p);
			if (vB != 0 && vB->code == 'Y'){
				vResult = true;
				vChoosed = true;
			}
			if (vB != 0 && vB->code == 'N'){
				vResult = false;
				vChoosed = true;
			}
		}
		delay(50);
	}

	//go back to previous panel
	gotoPanel(vPreviousPanel);
	return vResult;
}