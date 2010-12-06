

#include "Wire.h"
#include "BlinkM_funcs.h"

#define COLORLAMPDEBUG

// define this if you're plugging a BlinkM directly into an Arduino,
// into the standard position on analog in pins 2-5
// otherwise you can comment it out.
#define BLINKM_ARDUINO_POWERED 1
#define CMD_START_BYTE  0x01
int blinkm_addr = 0x09;

// Taken from http://kousaku-kousaku.blogspot.com/2008/08/arduino_24.html
//modified to match my sparkfun connector
#define xLow  17
#define xHigh 15
#define yLow  16
#define yHigh 14

#define STATIC_CALIBRATION_MIN_X 100
#define STATIC_CALIBRATION_MAX_X 700
#define STATIC_CALIBRATION_MIN_Y 100
#define STATIC_CALIBRATION_MAX_Y 700

#define HSB_BRIGHTNESS_ZONE_SIZE 220

//store the last HSB like 0x??HHSSBB
long mLastHSB = 0x000000FF; //the current value, could be altered by effects
long mLastUserDefinedHSB = 0x000000FF; //the real value the user defined

double TemplateW = 300;
double TemplateH = 300;

void ledOFF(){
//todo
}

//Sets a color by HSB
void SetHSB(byte pHue, byte pSaturation, byte pBrightness){
  Wire.beginTransmission(blinkm_addr);// join I2C, talk to BlinkM 0x09
  Wire.send('h'); // ÅehÅf == fade to HSB color
  Wire.send((uint8_t)pHue); // value for red channel
  Wire.send((uint8_t)pSaturation); // value for blue channel
  Wire.send((uint8_t)pBrightness); // value for green channel
  Wire.endTransmission(); // leave I2C bus
}

//Set color from HSB from the position touched
void HSBFromPosition(double x, double y){
  byte vHue = map(
    max(min(x, STATIC_CALIBRATION_MAX_X), HSB_BRIGHTNESS_ZONE_SIZE), 
    HSB_BRIGHTNESS_ZONE_SIZE, 
    STATIC_CALIBRATION_MAX_X, 
    0, 
    255);
  byte vSaturation = map(
    max(min(y, STATIC_CALIBRATION_MAX_Y), STATIC_CALIBRATION_MIN_Y), 
    STATIC_CALIBRATION_MIN_Y, 
    STATIC_CALIBRATION_MAX_Y, 
    0, 
    255);
  byte vBrightness = (mLastUserDefinedHSB & 0x000000FF); 

  //IF when clicked on the brightness zone, we override the results
  if (x < HSB_BRIGHTNESS_ZONE_SIZE){
    //y coordinagtes becomes the brightnes
    //just change brigthness according the y
    vBrightness = vSaturation;

    //get previous HSB from saved val
    vHue = (mLastHSB & 0x00FF0000) >> 16;   
    vSaturation = (mLastHSB & 0x0000FF00) >> 8;   
    
#ifdef COLORLAMPDEBUG 
    Serial.print("Change Brigthness only to ");
#endif
  }
  else {
#ifdef COLORLAMPDEBUG 
    Serial.print("Set Hue & Sat to ");
#endif
  }  

  //in case ... seems the map function is not very collaborative sometimes
  if (vHue > 255) vHue = 255;
  if (vSaturation > 255) vSaturation = 255;
  if (vBrightness > 255) vBrightness = 255;
  
  //save the last HSB
  mLastHSB = (((long)vHue) << 16) |  (((long)vSaturation) << 8) | (long)vBrightness;
  mLastUserDefinedHSB = mLastHSB;
  
#ifdef COLORLAMPDEBUG 
    Serial.print("HSB {");
    Serial.print(vHue,DEC);   
    Serial.print(" ");     
    Serial.print(vSaturation,DEC);   
    Serial.print(" ");     
    Serial.print(vBrightness,DEC);   
    Serial.println("}"); 
    /*
    Serial.print("HSB from saved ");
    Serial.print((mLastHSB & 0x00FF0000) >> 16,DEC);   
    Serial.print(" ");     
    Serial.print((mLastHSB & 0x0000FF00) >> 8,DEC);   
    Serial.print(" ");     
    Serial.print((mLastHSB & 0x000000FF),DEC);   
    Serial.println(""); 
    */
#endif

  SetHSB(vHue, vSaturation, vBrightness);
}

//Color effect : take the current color and change 
void ColorEffect_CandleLike (){
  byte vHue = (mLastHSB & 0x00FF0000) >> 16;
  byte vSaturation = (mLastHSB & 0x0000FF00) >> 8;  
  byte vBrightness = (mLastHSB & 0x000000FF);  
  
  //get the original brigthness
  byte vOriginalBrightness = (mLastUserDefinedHSB & 0x000000FF); 
  byte vRnd = random(1, 12);
  
  if (vBrightness > vOriginalBrightness){
    //over : go down
    vBrightness = max(0, vBrightness - vRnd);
  }
  else {
    //under : go up
    vBrightness = min(255, vBrightness + vRnd);
  }
  
  //save last hsb
  mLastHSB = (((long)vHue) << 16) |  (((long)vSaturation) << 8) | (long)vBrightness;
  SetHSB(vHue, vSaturation, vBrightness);
}

//Makes the light Red, green and then blue for a small duration
void helloRGB(){

Wire.beginTransmission(blinkm_addr);// join I2C, talk to BlinkM 0x09
Wire.send('n'); // ÅecÅf == fade to color
Wire.send(255); // value for red channel
Wire.send(0); // value for blue channel
Wire.send(0); // value for green channel
Wire.endTransmission(); // leave I2C bus
delay(250);

Wire.beginTransmission(blinkm_addr);// join I2C, talk to BlinkM 0x09
Wire.send('n'); // ÅecÅf == fade to color
Wire.send(0); // value for red channel
Wire.send(255); // value for blue channel
Wire.send(0); // value for green channel
Wire.endTransmission(); // leave I2C bus
delay(250);

Wire.beginTransmission(blinkm_addr);// join I2C, talk to BlinkM 0x09
Wire.send('n'); // ÅecÅf == fade to color
Wire.send(0); // value for red channel
Wire.send(0); // value for blue channel
Wire.send(255); // value for green channel
Wire.endTransmission(); // leave I2C bus
delay(250);

Wire.beginTransmission(blinkm_addr);// join I2C, talk to BlinkM 0x09
Wire.send('c'); // ÅecÅf == fade to color
Wire.send(0); // value for red channel
Wire.send(0); // value for blue channel
Wire.send(0); // value for green channel
Wire.endTransmission(); // leave I2C bus
delay(250);
}

//initializes the BlinkM
void setupBlinkM(){
    if( BLINKM_ARDUINO_POWERED ) {
#ifdef COLORLAMPDEBUG
    Serial.println("Call BlinkM_beginWithPower()"); 
#endif
      BlinkM_beginWithPower();
    } else {
#ifdef COLORLAMPDEBUG
    Serial.println("Call BlinkM_begin()"); 
#endif
        BlinkM_begin();
    }
     
    byte rc = BlinkM_checkAddress( blinkm_addr );
#ifdef COLORLAMPDEBUG
    Serial.print("rc=");
    Serial.println(rc, DEC); 
#endif
    
    if (rc == 0){
      //found !
      Wire.beginTransmission(blinkm_addr);// join I2C, talk to BlinkM 0x09
      Wire.send('o'); // ÅeoÅf == stop playing script !!

      Wire.send('f'); // ÅefÅf == set fade speed
      Wire.send((uint8_t)80); // the speed 1..255 with one slowest and 255 instantaneous
            
      Wire.endTransmission(); // leave I2C bus
      delay(50); //in case
    }
}

void setup(){
#ifdef COLORLAMPDEBUG  
  Serial.begin(9600);
  Serial.println("STARTUP");
#endif

  //first, init the communication
  Wire.begin(); // set up I2C
  
  //reset address (in case, most of the time leave commented)
  //BlinkM_setAddress(blinkm_addr);
#ifdef COLORLAMPDEBUG  
  Serial.print("Address: ");
  byte num = BlinkM_getAddress( 0 );
  Serial.println(num, DEC);
#endif

  setupBlinkM();
  
  helloRGB();


}

void loop(){
  pinMode(xLow,OUTPUT);
  pinMode(xHigh,OUTPUT);
  digitalWrite(xLow,LOW);
  digitalWrite(xHigh,HIGH);

  digitalWrite(yLow,LOW);
  digitalWrite(yHigh,LOW);

  pinMode(yLow,INPUT);
  pinMode(yHigh,INPUT);
  delay(10);

  //xLow has analog port -14 !!
  int x=analogRead(yLow -14);
  
  pinMode(yLow,OUTPUT);
  pinMode(yHigh,OUTPUT);
  digitalWrite(yLow,LOW);
  digitalWrite(yHigh,HIGH);

  digitalWrite(xLow,LOW);
  digitalWrite(xHigh,LOW);

  pinMode(xLow,INPUT);
  pinMode(xHigh,INPUT);
  delay(10);

  //xLow has analog port -14 !!
  int y=analogRead(xLow - 14);


  if (x <= 10 || y <= 10){
    //ledOFF();
  }
  else {

#ifdef COLORLAMPDEBUG
    Serial.print(x,DEC);   
    Serial.print(",");     
    Serial.println(y,DEC); 
#endif
    
    HSBFromPosition(x, y);
  }
  
  //little effect
  //ColorEffect_CandleLike ();
  
  delay(100);
}
