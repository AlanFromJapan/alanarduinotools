
#define USE_SERIAL

//Wire.h needed for the LcdDisplay, must be declared here
#include <Wire.h>

#include "HeadAxisControl.h"
#include "Test_Head.h"
#include "HeadGlobals.h"
#include "BedGlobals.h"
#include "BedControl.h"
#include "Joystick.h"
#include "LcdDisplay.h"


// the setup routine runs once when you press reset:
void setup() {
#ifdef USE_SERIAL  
  Serial.begin(115200);
#endif //USE_SERIAL

  setupLcdDisplay();
  
  
  lcdShowStartupScreen();
  delay(3000);
  lcdClear();
  lcdShowPositionAdjustScreen();
  
  setupHeadPositionControl();
  setupHeadStopperInterrupt();

  setHeadLeftmost();
  resetBedToStopper();

#ifdef USE_SERIAL

  Serial.println("==========================================================");
  Serial.print("HEAD POS=");Serial.println(mHeadPos);
  Serial.print("BED POS=");Serial.println(mBedPos);
  Serial.println("");


#endif //USE_SERIAL

  initHeadPositionJoystick();

}

// the loop routine runs over and over again forever:
void loop() {

#ifdef USE_SERIAL
  if (Serial.available()){
    byte vChar = Serial.read();
    
    switch (vChar){
      case 'l':
        moveHeadByAmount(-300, PWMSPEED);
        break;
      case 'r':
        moveHeadByAmount(300, PWMSPEED);
        break;
      case 'L':
        moveHeadByAmount(-1000, PWMSPEED_FAST);
        break;
      case 'R':
        moveHeadByAmount(1000, PWMSPEED_FAST);
        break;
      case 'M':
        moveHeadByAmount(4000, PWMSPEED_FAST);
        break;
        
        
      case 'n':
        moveBedNorth(15);
        break;
      case 's':
        moveBedSouth(15);
        break;        
      case 'N':
        moveBedNorth(100);
        break;
      case 'S':
        moveBedSouth(100);
        break;        
      case '6':
        moveBedNorth(6);
        break;        
      case '7':
        moveBedNorth(3);
        break;
        
      case'1':
        testMakeSquare();
        break;
      case'3':
        testMakeTriangle();
        break;
      case'4':
        testMakeStar();
        break;
      case '0':
        setHeadLeftmost();
        resetBedToStopper();      
        break;
    }
  }
#endif //USE_SERIAL  
}


void testMakeSquare(){
  //moveBedNorth(100);  
  moveHeadByAmount(700+143, PWMSPEED_FAST);
  
  delay(5000);
  moveHeadByAmount(143, PWMSPEED);
 
  delay(1000);
  moveBedSouth(60);  

  delay(1000);
  moveHeadByAmount(-143, PWMSPEED);
  
  delay(1000);
  moveBedNorth(60);  
}

void testMakeTriangle(){
    moveBedToPosition(0);
    moveHeadToPosition(800);  
    
  delay(5000);
  
    line(mBedPos, mHeadPos, 120, 1100);
    line(mBedPos, mHeadPos, 120, 800);
    line(mBedPos, mHeadPos, 0, 800);
}

void testMakeStar(){
    moveBedToPosition(-100);
    moveHeadToPosition(800);  
    
  delay(5000);

setHeadSpeed(PWMSPEED_ADJUST);

    lintTo(-200, 2000);
    lintTo(-300, 800);
    lintTo(-100, 1700);
    lintTo(-300, 1700);
    lintTo(-100, 800);
}

void lintTo (int tox, int toy){
  line(mBedPos, mHeadPos, tox, toy);
}

//Bresenham algorithm
// Thank you wikipedia http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void line (int fromx, int fromy, int tox, int toy){
  int dx = abs(tox - fromx);
  int dy = abs(toy - fromy);
  int sy, sx, err, err2;
  
  if (fromx < tox) sx = 1; else sx = -1;
  if (fromy < toy) sy = 1; else sy = -1;
  
  err = dx-dy;
  
  while (1){
    //X is the bed, Y is the head
    moveBedToPosition(fromx);
    moveHeadToPosition(fromy);
    
    if (fromx == tox && fromy == toy) break;
      
    err2 = err * 2;
    if (err2 > -dy){
      err = err - dy;
      fromx = fromx + sx;
    }

    if (fromx == tox && fromy == toy) {
      moveBedToPosition(fromx);
      moveHeadToPosition(fromy);
      break;
    }
    
    if (err2 < dx){
      err = err + dx;
      fromy = fromy + sy;
    }
  }
}

//moves the head following joystick
void initHeadPositionJoystick(){
  while (
    1 /*change condition later*/ 
#ifdef USE_SERIAL    
    && !Serial.available()
#endif //USE_SERIAL    
  ){
    int8_t x, y;
    readJoystick (&x, &y);

    if (x != 0){
      moveHeadByAmount(x * MILLI_HEAD_STEPS, PWMSPEED_ADJUST);
    }    
    
    if (y != 0){
      moveBedToPosition(mBedPos + y * MILLI_BED_STEPS);
    }    

  }
}


