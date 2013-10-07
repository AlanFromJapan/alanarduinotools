#include "HeadAxisControl.h"
#include "Test_Head.h"
#include "HeadGlobals.h"
#include "BedGlobals.h"
#include "BedControl.h"

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  
  setupPositionControl();
  setupHeadStopperInterrupt();
  setHeadLeftmost();
  resetBedToStopper();
  
//delay(3000);
//moveHeadByAmount(+4000);

//testMakeSquare();
}

// the loop routine runs over and over again forever:
void loop() {
/*
  moveHeadByAmount(200);
  delay(500);

  moveHeadByAmount(-500);
  delay(500);

  moveHeadByAmount(600);
  delay(500);

  moveHeadByAmount(-300);
  delay(500);
  
  moveHeadToPosition(0);
  Serial.println("==========================================================");
  Serial.print("Reset ot origin ;POS=");Serial.println(mHeadPos);
  Serial.println("");
  
  delay(5000);
*/
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


