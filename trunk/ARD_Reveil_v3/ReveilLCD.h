#include "WProgram.h"
#include "wiring.h"

#ifndef __REVEILLCD_H__
#define __REVEILLCD_H__



#include "LCD4Bit.h"
LCD4Bit lcd = LCD4Bit(2); 
#define LCD_BACKLIGHT 9
#define LCD_DEFAULT_VAL 0

int mBacklightValue = LCD_DEFAULT_VAL;

//Background light : ligten 
void backlightLighten(){
  //lighten ...
  for (int i = mBacklightValue; i < 256 ; i += 15){
    analogWrite(LCD_BACKLIGHT, i);
    delay(35);
  }
}

//Background light : darken
void backlightDarken(){
  //darken ...
  for (int i = 255; mBacklightValue < i ; i -= 15){
    analogWrite(LCD_BACKLIGHT, i);
    delay(35);
  }

  //reset to default
  analogWrite(LCD_BACKLIGHT, mBacklightValue);
}

void showVersion(){
  lcd.cursorTo(1, 0);
  lcd.printIn("  ardReveil III ");
  lcd.cursorTo(2, 0);
  lcd.printIn("    v 1.0.0     "); 
  
  delay(1000);
  lcd.leftScroll(16, 50);

  lcd.cursorTo(1, 0);
  lcd.printIn("Special  version");
  lcd.cursorTo(2, 0);
  lcd.printIn("(^o^) RiSu (^o^)"); 
}


//prepares the display for the usual screen with time.
//not meant to be called each second
void lcdMakeDefaultScreen(){
  //lcd.clear();
  //lcd.cursorTo(1, 0);
  //lcd.printIn("                ");
  lcd.cursorTo(2, 0);
  
  int vRnd = random(0, 10);
  switch (vRnd){
    case 0:
    lcd.printIn("     (^_~)b     "); 
    break;
    case 1:
    lcd.printIn("    (o^-^o)     "); 
    break;
    case 2:
    lcd.printIn("     (-.-;)     "); 
    break;
    case 3:
    lcd.printIn("Courage  p(^^)q "); 
    break;
    case 4:
    lcd.printIn("Love!  (^3^)--* "); 
    break;
    case 5:
    lcd.printIn("    \\(^O^)/     "); 
    break;
    case 6:
    lcd.printIn(" Yeah  (o^-' )b "); 
    break;
    case 7:
    lcd.printIn(" (-_-) Zzzzzzz  "); 
    break;
    case 8:
    lcd.printIn("Reveil  Ecureuil"); 
    break;
    case 9:
    lcd.printIn(" Special  RiSu! "); 
    break;
  }
}

//writes a line of text
void lcdWriteLine(byte pLine, char* pText){
  lcd.cursorTo(pLine, 0);
  lcd.printIn(pText);
}

//does some sort of splash screen
void splashScreen(){
  lcd.clear();
  delay(1000);

  //get some light
  backlightLighten();

  //show the version
  showVersion();

  delay(1000);
  lcd.leftScroll(16, 50);
  
  lcdMakeDefaultScreen();
  
  //fade to black
  backlightDarken();
}

void setDefaultBrightness(int pVal){
  mBacklightValue = pVal;
}

void setupLCD(){
  int vPins[] = {
    3,2,1,0    };
  //(int pPins[], int pRS, int pRW, int pEnable)
  lcd.overrideLCDPins(vPins, 7, 99, 5);

  lcd.init();
  lcd.clear();

  splashScreen();
}

#endif //__REVEILLCD_H__
