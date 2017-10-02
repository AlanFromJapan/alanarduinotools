
#define PINSIN1   2
#define PINSIN2   3
#define PINSIN3   4
#define PINCLOCK  5
#define PINLATCH  6
#define PINSTROBE 7

#define DELAYUS   5


void test1() {
  //GO!
  digitalWrite(PINSIN1, HIGH);
  delayMicroseconds (DELAYUS);
  digitalWrite(PINCLOCK, HIGH);
  delayMicroseconds (DELAYUS);
  
  digitalWrite(PINSIN1, LOW);
  delayMicroseconds (DELAYUS);
  digitalWrite(PINCLOCK, LOW);
  delayMicroseconds (DELAYUS);
  
  for (int i =0; i < 15; i++) {
    digitalWrite(PINSIN2, (i % 2 == 0 ? HIGH : LOW));
    digitalWrite(PINSIN3, (i % 2 == 0 ? LOW : HIGH));
    delayMicroseconds (DELAYUS);

    digitalWrite(PINCLOCK, HIGH);
    delayMicroseconds (DELAYUS);
    
    digitalWrite(PINSIN2, LOW);
    digitalWrite(PINSIN3, LOW);
    delayMicroseconds (DELAYUS);
    
    digitalWrite(PINCLOCK, LOW);
    delayMicroseconds (DELAYUS);    
  }

  digitalWrite(PINLATCH, LOW);
  delayMicroseconds (DELAYUS * 3);
  digitalWrite(PINLATCH, HIGH);
  delayMicroseconds (DELAYUS);
}


int b = 0;

void test2() {  
  for (int line =0; line < 16; line++){ 
    //NEW LINE!
    digitalWrite(PINSIN1, HIGH);
    
    for (int i =0; i < 16; i++) {

      //set leds
      if (i == b) {
        digitalWrite(PINSIN2, HIGH);
        digitalWrite(PINSIN3, HIGH);
      }
      delayMicroseconds (DELAYUS);

      //read!
      digitalWrite(PINCLOCK, HIGH);
      delayMicroseconds (DELAYUS);

      //go down      
      digitalWrite(PINSIN2, LOW);
      digitalWrite(PINSIN3, LOW);      
      digitalWrite(PINSIN1, LOW);
      
      digitalWrite(PINCLOCK, LOW);
    }

    //delayMicroseconds (DELAYUS);    
    digitalWrite(PINLATCH, LOW);
    delayMicroseconds (DELAYUS);
/*    
    //read!
    digitalWrite(PINCLOCK, HIGH);
    delayMicroseconds (DELAYUS);
    digitalWrite(PINCLOCK, LOW);
    delayMicroseconds (DELAYUS);
*/          
    digitalWrite(PINLATCH, HIGH);
    delayMicroseconds (DELAYUS);
      
  }
}




int test3delais = 0;
void test3() {  

  test3delais++;
  if (test3delais >= 100){
    b = (b+1) % 16;
    test3delais = 0;
  }
    
  for (int y =0; y < 16; y++){ 
    
    for (int x =0; x < 16; x++) {
      if (x == y) {
        //NEW LINE!
        digitalWrite(PINSIN1, HIGH);
      }
      
      //set leds
      if (x == b) {
        digitalWrite(PINSIN2, HIGH);
        digitalWrite(PINSIN3, HIGH);
      }
      delayMicroseconds (DELAYUS);

      //read!
      digitalWrite(PINCLOCK, HIGH);
      delayMicroseconds (DELAYUS);

      //go down      
      digitalWrite(PINSIN2, LOW);
      digitalWrite(PINSIN3, LOW);      
      digitalWrite(PINSIN1, LOW);
      
      digitalWrite(PINCLOCK, LOW);
    }

    //delayMicroseconds (DELAYUS);    
    digitalWrite(PINLATCH, LOW);
    delayMicroseconds (DELAYUS);
/*    
    //read!
    digitalWrite(PINCLOCK, HIGH);
    delayMicroseconds (DELAYUS);
    digitalWrite(PINCLOCK, LOW);
    delayMicroseconds (DELAYUS);
*/
    digitalWrite(PINLATCH, HIGH);
    delayMicroseconds (DELAYUS);

      
  }

}



/**********************************************************************************************************************************************************/
 const unsigned char mbed[] = {
 0xff,0xff,0xff,0xff,  // D15   
 0xff,0xff,0xff,0xff,  // D14
 0xff,0xff,0xff,0xff,  // D13
 0xff,0xe7,0xff,0xfc,  // D12
 0xff,0xe7,0xff,0xfc,  // D11
 0xff,0xe7,0xff,0xfc,  // D10
 0x44,0x60,0xe0,0xe0,  // D9
 0x00,0x20,0x40,0x40,  // D8
 0x33,0x26,0x4e,0x4c,  // D7
 0x33,0x26,0x40,0x4c,  // D6
 0x33,0x26,0x4f,0xcc,  // D5
 0x33,0x20,0x60,0xc0,  // D4
 0x33,0x28,0xf1,0xe4,  // D3
 0xff,0xff,0xff,0xff,  // D2
 0xff,0xff,0xff,0xff,  // D1
 0xff,0xff,0xff,0xff}; // D0


unsigned char mMatrix[64];

int test4delais = 0;
void test4( unsigned char buf[]) {  

  test4delais++;
  if (test4delais >= 100){
    b = (b+1) % 16;
    test4delais = 0;
  }
    
  for (int y =0; y < 16; y++){ 
    uint16_t l = (buf [4*y] << 8) + buf [4*y +1];
    uint16_t r = (buf [4*y+2] << 8) + buf [4*y+2 +1];
    
    for (int x =0; x < 16; x++) {
      if (x == y) {
        //NEW LINE!
        digitalWrite(PINSIN1, HIGH);
      }
      
      //set leds
      if ((l & (1 <<x)) != 0)
        digitalWrite(PINSIN2, HIGH);
      else
        digitalWrite(PINSIN2, LOW);

      if ((r & (1 <<x)) != 0)
        digitalWrite(PINSIN3, HIGH);
      else
        digitalWrite(PINSIN3, LOW);

      delayMicroseconds (DELAYUS);

      //read!
      digitalWrite(PINCLOCK, HIGH);
      delayMicroseconds (DELAYUS);

      //go down      
      digitalWrite(PINSIN2, LOW);
      digitalWrite(PINSIN3, LOW);      
      digitalWrite(PINSIN1, LOW);
      
      digitalWrite(PINCLOCK, LOW);
    }

    //delayMicroseconds (DELAYUS);    
    digitalWrite(PINLATCH, LOW);
    delayMicroseconds (DELAYUS);
/*    
    //read!
    digitalWrite(PINCLOCK, HIGH);
    delayMicroseconds (DELAYUS);
    digitalWrite(PINCLOCK, LOW);
    delayMicroseconds (DELAYUS);
*/
    digitalWrite(PINLATCH, HIGH);
    delayMicroseconds (DELAYUS);

      
  }

}

void test4rnd(unsigned char buf[]){
  toggleBit(buf, random(32), random (16));
}

void resetBuf (unsigned char buf[]) {
  for (uint8_t i = 0 ; i < 64; i++)
    buf[i] = 0;  
}

void toggleBit (unsigned char buf[], uint8_t x, uint8_t y) {
  uint8_t v = ((y * 32) +x) / 8;
  buf[v] = buf[v] ^ (x % 8);
}
/**********************************************************************************************************************************************************/


 
void setup() {
  // put your setup code here, to run once:
  pinMode(PINCLOCK, OUTPUT);
  pinMode(PINSIN1, OUTPUT);
  pinMode(PINSIN2, OUTPUT);
  pinMode(PINSIN3, OUTPUT);
  pinMode(PINLATCH, OUTPUT);
  pinMode(PINSTROBE, OUTPUT);

  digitalWrite(PINCLOCK, LOW);
  digitalWrite(PINSIN1, LOW);
  digitalWrite(PINSIN2, LOW);
  digitalWrite(PINSIN3, LOW);
  digitalWrite(PINLATCH, HIGH);
  digitalWrite(PINSTROBE, LOW);



}

void loop() {
  test4rnd(mMatrix);
  test4(mMatrix);


  
}
