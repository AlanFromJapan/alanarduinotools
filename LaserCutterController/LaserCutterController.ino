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
        
      case'1':
        testMakeSquare();
        break;
      case '0':
        setHeadLeftmost();
        resetBedToStopper();      
        break;
    }
  }
}

/*
void testMove (){
   while (1) {
    // change the analog out value:
    if (vLeftRight){
      analogWrite(3, PMWSPEED);           
    }
    else {
      analogWrite(PWM_PIN_LEFT, PMWSPEED);           
    }
    
    while (mPos < abs(200)) {
      delay(PWMSTEP);
    }
    
    //stop
    analogWrite(3, 0);           
    analogWrite(PWM_PIN_LEFT, 0);   
      
    vLeftRight = !vLeftRight;

    Serial.print("POS=");Serial.println(mPos);
    delay(1000);  
  }  
}
*/

void testMakeSquare(){
  moveBedNorth(100);  
  moveHeadByAmount(500, PWMSPEED);
  
  delay(5000);
  moveHeadByAmount(1000, PWMSPEED);
 
  delay(1000);
  moveBedSouth(200);  

  delay(1000);
  moveHeadByAmount(-1000, PWMSPEED);
  
  delay(1000);
  moveBedNorth(200);  
}




