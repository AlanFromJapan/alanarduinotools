
#define PINSIN1   2
#define PINSIN2   3
#define PINSIN3   4
#define PINCLOCK  5
#define PINLATCH  6
#define PINSTROBE 7

#define DELAYUS   5

//https://xantorohara.github.io/led-matrix-editor/#7c7c6c6c6c6c7c7c|1818181818181818|7c7c0c7c7c607c7c|7c7c603838607c7c|6060607c7c6c6c0c|7c7c607c7c0c7c7c|7c7c6c7c7c0c0c0c|6060606060607c7c|7c7c6c7c7c6c7c7c|6060607c7c6c7c7c
const byte IMAGES[][8] = {
{
  B00111110,
  B00111110,
  B00110110,
  B00110110,
  B00110110,
  B00110110,
  B00111110,
  B00111110
},{
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00011000
},{
  B00111110,
  B00111110,
  B00000110,
  B00111110,
  B00111110,
  B00110000,
  B00111110,
  B00111110
},{
  B00111110,
  B00111110,
  B00000110,
  B00011100,
  B00011100,
  B00000110,
  B00111110,
  B00111110
},{
  B00110000,
  B00110110,
  B00110110,
  B00111110,
  B00111110,
  B00000110,
  B00000110,
  B00000110
},{
  B00111110,
  B00111110,
  B00110000,
  B00111110,
  B00111110,
  B00000110,
  B00111110,
  B00111110
},{
  B00110000,
  B00110000,
  B00110000,
  B00111110,
  B00111110,
  B00110110,
  B00111110,
  B00111110
},{
  B00111110,
  B00111110,
  B00000110,
  B00000110,
  B00000110,
  B00000110,
  B00000110,
  B00000110
},{
  B00111110,
  B00111110,
  B00110110,
  B00111110,
  B00111110,
  B00110110,
  B00111110,
  B00111110
},{
  B00111110,
  B00111110,
  B00110110,
  B00111110,
  B00111110,
  B00000110,
  B00000110,
  B00000110
}};
const int IMAGES_LEN = sizeof(IMAGES)/8;



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
void renderMatrix( unsigned char buf[]) {  

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
  buf[v] = buf[v] ^ (0x80 >> (x % 8));
}

void setBit (unsigned char buf[], uint8_t x, uint8_t y) {
  uint8_t v = ((y * 32) +x) / 8;
  buf[v] = buf[v] | (0x80 >> (x % 8));
}

void offBit (unsigned char buf[], uint8_t x, uint8_t y) {
  uint8_t v = ((y * 32) +x) / 8;
  buf[v] = buf[v] & ~(0x80 >> (x % 8));
}

uint8_t getBit (unsigned char buf[], uint8_t x, uint8_t y) {
  uint8_t v = ((y * 32) +x) / 8;
  if (  (buf[v] & (0x80 >> (x % 8))) != 0)
    return 1;
  else 
    return 0;
}

uint8_t mxIsEmpty (unsigned char buf[]) {
  for (uint8_t i = 0; i < 64; i++)
    if (buf[i] != 0)
      return 0;

  return 1;
}


int8_t mX = random(32);int8_t mY = random(16);int8_t mDX = random(3) -1;int8_t mDY = random(3) -1;
//int8_t mX = 0;int8_t mY = 0;int8_t mDX = 0;int8_t mDY = 0;
uint8_t mMoveBallDelay = 0;

void moveBall (unsigned char buf[]){
  mMoveBallDelay++;
  if (mMoveBallDelay < 10)
    return;
  mMoveBallDelay = 0;
  

  
  //offBit (buf, mX, mY);
  
  mX += mDX;
  mY += mDY;

  if (mX == 0 || mX == 31)
    mDX = -mDX;
  if (mY == 0 || mY == 15)
    mDY = -mDY;

  //setBit(buf, mX, mY);

toggleBit(buf, mX, mY);
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

uint16_t mReset = 0;
void anim1() {
  //test4rnd(mMatrix); renderMatrix(mMatrix);

  moveBall (mMatrix); renderMatrix(mMatrix);

  //renderMatrix (mbed);

  mReset++;
  if (mReset == 2000) {
    mReset = 0;
    mX = random(32);
    mY = random(16);
    mDX = random(3) -1;
    mDY = random(3) -1;  

    resetBuf(mMatrix);
  }  
}


void digit2Matrix (uint16_t v, uint8_t vOffset, unsigned char buf[]){
  for (uint8_t i = 0; i < 4; i++) {
    uint8_t digitId = (v%10000)/1000;
    for (uint8_t j = 0; j < 8; j++) {
      buf[(j + vOffset) * 4 + i] = IMAGES[digitId][j];      
    }
    v = (v - digitId * 1000)*10;
  }  
}

void anim2(uint16_t v) {
  uint8_t voffset = random(8);
  digit2Matrix(v, voffset, mMatrix);

  uint8_t count = 0;
  while (mxIsEmpty(mMatrix) == 0) {
    renderMatrix(mMatrix);  

    count++;
    if (count >= 5) {
      count = 0;
      for (uint8_t i = 0; i < 32 -1; i++) {
        for (uint8_t j = 0; j < 16; j++) {
          if (getBit (mMatrix, i+1, j) == 0)
            offBit(mMatrix, i, j);
          else
            setBit(mMatrix, i, j);
        }
      }        
    }
  }
}

uint16_t vCounter = 0;

void loop() {
  //anim2(vCounter++);

  digit2Matrix(vCounter++, 0, mMatrix);
  renderMatrix(mMatrix); 
  
}
