#include "HeadAxisControl.h"

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  
  setupPositionControl();

}

// the loop routine runs over and over again forever:
void loop() {

  moveByAmount(200);
  delay(500);

  moveByAmount(-500);
  delay(500);

  moveByAmount(600);
  delay(500);

  moveByAmount(-300);
  delay(500);
  
  moveToPosition(0);
  Serial.println("==========================================================");
  Serial.print("Reset ot origin ;POS=");Serial.println(mHeadPos);
  Serial.println("");
  
  delay(5000);

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




