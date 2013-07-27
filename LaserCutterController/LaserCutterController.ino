
#define PWM_PIN_LEFT  3
#define PWM_PIN_RIGHT 9

#define PMWSPEED          50
#define PMWSPEED_ADJUST   20
#define PWMSTEP           30
#define PWMSTEPSMALL      5

volatile int mPos = 0;
volatile boolean vLeftRight = true;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the pushbutton's pin an input:
//  pinMode(PINA, INPUT);  pinMode(PINB, INPUT);


DDRC &= 0xFC;     // Clear the PC0 and c1 pin
// PB0 (PCINT0 pin) is now an input

PORTC |= 0x03;    // turn On the Pull-up
// PB0 is now an input with pull-up enabled


PCICR = (1 << PCIE1);    // set PCIE0 to enable PCMSK0 scan
PCMSK1 = (1 << PCINT8) | (1 << PCINT9) ;  // set PCINT0 to trigger an interrupt on state change 


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
  Serial.print("Reset ot origin ;POS=");Serial.println(mPos);
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

byte toggleDirection (){
  //first stop
  analogWrite(PWM_PIN_LEFT, 0);
  analogWrite(PWM_PIN_RIGHT, 0);

  //reverse
  vLeftRight = !vLeftRight;

  //return the right PIN
  if (vLeftRight){
    return PWM_PIN_LEFT;
  }
  else {
    return PWM_PIN_RIGHT;    
  }  
}

void moveToPosition (int pTargetPosition){
  moveByAmount (pTargetPosition - mPos);
}

void moveByAmount (int pDistance){
  
  Serial.print("start moveByAmount(");Serial.print(pDistance);Serial.print(");POS=");Serial.println(mPos);
    
  vLeftRight = (pDistance > 0 ? true : false);
  unsigned int vCurrentDistance = 0;
  unsigned int vTargetDistance = abs(pDistance);
  int vStartPos = mPos;
  byte vPin = 0;
  
  // change the analog out value:
  if (vLeftRight){
    vPin = PWM_PIN_LEFT;
  }
  else {
    vPin = PWM_PIN_RIGHT;    
  }
  //start moving
  analogWrite(vPin, PMWSPEED);       
  
  //move
  while (vCurrentDistance < vTargetDistance) {
    delay(PWMSTEPSMALL);
    vCurrentDistance = abs(vStartPos - mPos);

    //Serial.print("vCurrentDistance");Serial.println(mPos);
  }
  
  if (vCurrentDistance > vTargetDistance) {
    Serial.print("  Too far ! POS=");Serial.println(mPos);

    //in case we moved too far, get back just a little
    vPin = toggleDirection();
    //change speed
    analogWrite(vPin, PMWSPEED_ADJUST); 
    while (vCurrentDistance > vTargetDistance) {
      delay(1);
      vCurrentDistance = abs(vStartPos - mPos);

      //Serial.print("  Too far ! POS=");Serial.println(mPos);
    }
  }
  
  //stop
  analogWrite(vPin, 0);       

  Serial.print("finish moveByAmount(");Serial.print(pDistance);Serial.print(");POS=");Serial.println(mPos);
}

//Handle the pin status change on the ADC0-1
ISR (PCINT1_vect)
{
    if (vLeftRight)
      mPos +=1;
    else 
      mPos -=1;
}




