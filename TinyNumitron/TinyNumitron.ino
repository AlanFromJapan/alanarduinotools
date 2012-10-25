//**************************************************************//
//  Name    : shiftOutCode, Hello World                                
//  Author  : Carlyn Maw,Tom Igoe, David A. Mellis 
//  Date    : 25 Oct, 2006    
//  Modified: 23 Mar 2010                                 
//  Version : 2.0                                             
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255                           
//****************************************************************

//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;


#define DIGIT0       0b00000100
#define DIGIT1       0b00011111
#define DIGIT2       0b00101000
#define DIGIT3       0b00001001
#define DIGIT4       0b00010011
#define DIGIT5       0b01000001
#define DIGIT6       0b01000000
#define DIGIT7       0b00001111
#define DIGIT8       0b00000000
#define DIGIT9       0b00000001
#define DIGITA       0b00000010
#define DIGITE       0b01100000

uint8_t DIGITS[] = {
   DIGIT0, DIGIT1, DIGIT2, DIGIT3, DIGIT4, 
   DIGIT5, DIGIT6, DIGIT7, DIGIT8, DIGIT9 };

void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loopAll() {
  // count from 0 to 255 and display the number 
  // on the LEDs
  for (int numberToDisplay = 0; numberToDisplay < 256; numberToDisplay++) {
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);  

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(500);
  }
}

void loop99(){
   
   for (int i = 0; i < 100; i++){
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    //units
    shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[i % 10] | 0b10000000);  
    
    // shift out the bits:
    //tens
    shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[i / 10] | 0b10000000);  

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(500);
    
   }
}

void all00() {
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    //units
    shiftOut(dataPin, clockPin, MSBFIRST, 0xff);  
    
    // shift out the bits:
    //tens
    shiftOut(dataPin, clockPin, MSBFIRST, 0xff);  

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(1000);
}


void loop9999(){
   
   for (int i = 0; i < 10000; i++){
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    //units
    shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[i % 10]             | ( (i / 10) % 4 == 0 ? 0b10000000 : 0x00));  
    
    // shift out the bits:
    //tens
    shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[(i / 10) % 10]      | ( (i / 10) % 4 == 1 ? 0b10000000 : 0x00));  
    //hundreds
    shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[(i / 100) % 10]     | ( (i / 10) % 4 == 2 ? 0b10000000 : 0x00));  
    //thousands
    shiftOut(dataPin, clockPin, MSBFIRST, DIGITS[(i / 1000) % 10]    | ( (i / 10) % 4 == 3 ? 0b10000000 : 0x00));   

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(200);
    
   }
}

void loop(){
   loop9999();
   
}
