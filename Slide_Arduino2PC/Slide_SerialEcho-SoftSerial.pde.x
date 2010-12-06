#include <SoftwareSerial.h>
#define rxPin 3
#define txPin 2

#include <ctype.h>

#define bit9600Delay 84  
#define halfBit9600Delay 42
#define bit4800Delay 188 
#define halfBit4800Delay 94 


//SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

int SWread()
{
  byte val = 0;
  while (digitalRead(rxPin));
  //wait for start bit
  if (digitalRead(rxPin) == LOW) {
    delayMicroseconds(halfBit9600Delay);
    for (int offset = 0; offset < 8; offset++) {
     delayMicroseconds(bit9600Delay);
     val |= digitalRead(rxPin) << offset;
    }
    //wait for stop bit + extra
    delayMicroseconds(bit9600Delay); 
    delayMicroseconds(bit9600Delay);
    return val;
  }
}


void setup() {
  pinMode(rxPin, INPUT);//set recieve as an input
  pinMode(txPin, OUTPUT);//set transmit as an output
  
  //mySerial.begin(9600);
  Serial.begin(9600); // begin serial
}

void loop () {
  char myByte;
  
  /*
  myByte = mySerial.read();
  Serial.print((char)myByte);
  Serial.print(' ');
  Serial.println(myByte, DEC);
  delay(5);
  */
  
  

  myByte = (byte)SWread();
  Serial.print((char)myByte);
  Serial.print(' ');
  Serial.println(myByte, DEC);
  delay(5);
}
