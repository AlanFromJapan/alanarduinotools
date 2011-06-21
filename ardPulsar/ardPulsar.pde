
#define MODE_OFF 'S'
#define MODE_COLOR 'C'
#define MODE_FADE 'F'


#define DEBUG

struct LED {
  byte r;
  byte g;
  byte b;

  byte targetr;
  byte targetg;
  byte targetb;

  byte stepr;
  byte stepg;
  byte stepb;

  char mode;
};

LED leds[3];
int ledSetPin [3];

void setup() {                
  Serial.begin(9600);  

  pinMode(2, OUTPUT); 
  pinMode(4, OUTPUT); 
  pinMode(7, OUTPUT); 

  ledSetPin[0] =2;
  ledSetPin[1] =4;
  ledSetPin[2] =7;

  //change the speed of PWM of 11 and 3 to 32khz
  //TCCR2B = TCCR2B & 0b11111000 | 0x01;

  //change the speed of PWM of 5 and 6 to 64khz
  //TCCR0B = TCCR0B & 0b11111000 | 0x01;
}


byte WaitAndRead()
{
  while (Serial.available() == 0) {
    // do nothing
  }
  return (byte) Serial.read();
}

//http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1168215139
int SerialReadHexDigit()
{
  byte c = WaitAndRead();
  if (c >= '0' && c <= '9') {
    return c - '0';
  } 
  else if (c >= 'a' && c <= 'f') {
    return c - 'a' + 10;
  } 
  else if (c >= 'A' && c <= 'F') {
    return c - 'A' + 10;
  } 
  else {
#ifdef DEBUG
    Serial.println("Error reading color");
#endif
    return -1;   // getting here is bad: it means the character was invalid
  }
}

/**
 * Read command from the serial
 */
void readCommand(){
  if (Serial.available() >= 2){
    byte c = Serial.read();
    byte ledChar = Serial.read();
    int ledId = 100;

    switch(ledChar){
    case 'S':
      ledId = 1;
      break;
    case 'E':
      ledId = 0;
      break;
    case 'W':
      ledId = 2;
      break;
    }

    if (ledId == 100){
      //Error ?? flush
      Serial.flush();
#ifdef DEBUG
      Serial.print("unknown led id:");
      Serial.println(ledChar, BYTE);
#endif
    }
    else {
      switch (c){
      case MODE_OFF:
        leds[ledId].mode = MODE_OFF;

#ifdef DEBUG
        Serial.println("MODE_OFF");
#endif
        break;
      case MODE_COLOR:
        leds[ledId].mode = MODE_COLOR;
        leds[ledId].r = SerialReadHexDigit();
        leds[ledId].g = SerialReadHexDigit();
        leds[ledId].b = SerialReadHexDigit();

#ifdef DEBUG
        Serial.println("MODE_COLOR");
#endif
        break;
      }
    }

    //avoid further misreading, clear the buffer
    Serial.flush();
  }
}

void displayLed (int i){
  if (leds[i].mode == MODE_OFF){
    //just let off
    /*
    analogWrite(3, 0);
     analogWrite(5, 0);
     analogWrite(6, 0);
     
     digitalWrite(ledSetPin[i], HIGH);
     */
  }

  if (leds[i].mode == MODE_COLOR){
    //set color
    analogWrite(3, leds[i].r);
    analogWrite(5, leds[i].g);
    analogWrite(6, leds[i].b);

    //and turn on
    digitalWrite(ledSetPin[i], HIGH);
  }

}

void loop() {
  int vLed = 0;

  while(true){

    //don't read each turn, once every full round is enough
    if (vLed == 0){
      readCommand();
    }

    //turn all off
    digitalWrite(2, LOW);
    digitalWrite(4, LOW);
    digitalWrite(7, LOW);

    displayLed(vLed);

    //sleep a little for POV
    delay (3);

    //go next
    vLed = ++vLed % 3;
  }
}
















