/*
CameleonWand 
http://kalshagar.wikispaces.com/cameleon

Reads a color using a CdS photocell. Not perfect, far from repeatable but kinda works.
 */

#include <EEPROM.h>

#define xxxDEBUG 

//How many sample of RGB to read
#define SAMPLECOUNT 2
//How long is the light on before reading
#define READ_LIGHT_DURATION 100
//Cool down between two color component reading
#define READ_LINE_COOLDOWN 10

//Pin number of the Red pin of the color reading RGB
//Pin order is RGB with G=R+1 and B=R+2
#define RGB_START_PIN 5

//Start pin for the RGB output in the order R-G-B
#define RGB_OUT_PIN 9

//Where the default value first byte is stored
#define EEPROM_DFLT_START 0

int RGB_HIGHLOW [3][2];

void setup() {

#ifdef DEBUG
   Serial.begin(9600);
#else
    //factor settings is to divide internal clock 8MHz by 8.
    //don't, and just run at 8 MHz (set the clock divider to 1 so no effect)
    CLKPR = (1<<CLKPCE);
    CLKPR = 0; // Divide by 1 
#endif
   pinMode (4, OUTPUT);
   pinMode (5, OUTPUT);
   pinMode (6, OUTPUT);
   pinMode (7, OUTPUT);

   pinMode (9, OUTPUT);
   pinMode (10, OUTPUT);
   pinMode (11, OUTPUT);
   
   digitalWrite (4, LOW);

/*
   Serial.println ("Wait before defaults!");
   delay(10000);
   setDefaults();
   storeDefaults();
*/

   loadDefaults();
   showDefaults();

/*
int vColor1[3] = {255, 0, 0}; showColor( vColor1); delay(500);
int vColor2[3] = {0, 255, 0}; showColor( vColor2); delay(500);
int vColor3[3] = {0, 0, 255}; showColor( vColor3); delay(500);
*/

   delay(500);
}

void loop() {

   readColor();
   
   delay(2000);
}

void readColor (){
   int sensorValue;
#ifdef DEBUG
   Serial.println ("Check color.");
#endif
   int vColor [3];
   vColor[0] = 0;
   vColor[1] = 0;
   vColor[2] = 0;
   for (int j = 0; j < SAMPLECOUNT; j++) {
      for (int i = 0; i < 3; i++){
         digitalWrite (i+RGB_START_PIN, HIGH);
         delay(READ_LIGHT_DURATION);
         sensorValue = analogRead(A0);
         vColor[i] += sensorValue;
         digitalWrite (i+RGB_START_PIN, LOW); 
         delay(READ_LINE_COOLDOWN);     
      }
   }
   for (int i = 0; i < 3; i++){
      vColor[i] = vColor[i] / SAMPLECOUNT;
   }

#ifdef DEBUG
   Serial.print ("RAW RGB (");
   Serial.print (vColor[0]);
   Serial.print (";");
   Serial.print (vColor[1]);
   Serial.print (";");
   Serial.print (vColor[2]);
   Serial.print (")");
#endif
/*
//using map()) : get negative results !
   Serial.print ("Fixed RGB (");
   Serial.print (map(vColor[0], RGB_HIGHLOW[0][1], RGB_HIGHLOW[0][0], 0, 255));
   Serial.print (";");
   Serial.print (map(vColor[1], RGB_HIGHLOW[1][1], RGB_HIGHLOW[1][0], 0, 255));
   Serial.print (";");
   Serial.print (map(vColor[2], RGB_HIGHLOW[2][1], RGB_HIGHLOW[2][0], 0, 255));
   Serial.print (")");
   Serial.println ("");
   */
   
   int vMappedColor [3];
   for (int i =0; i< 3; i++){
      vMappedColor[i] = map2(vColor[i], RGB_HIGHLOW[i][0], RGB_HIGHLOW[i][1], 0, 255);
   }
   
#ifdef DEBUG
   Serial.print ("Fixed RGB (");
   Serial.print (vMappedColor[0]);
   Serial.print (";");
   Serial.print (vMappedColor[1]);
   Serial.print (";");
   Serial.print (vMappedColor[2]);
   Serial.print (")");
   Serial.println ("");
#endif   
   showColor(vMappedColor);
}

//If less than pThreshold returns 0
int limitLowColor (int pThreshold, int pValue){
   return (pValue < pThreshold ? 0 : pValue);
}

void showColor(int pColor []){
   //RED : dimish it, it's WAY too strong compared to pale blue
   analogWrite (RGB_OUT_PIN, limitLowColor(50, pColor[0] * 1 / 2));
   //GREEN : lower just a bit
   analogWrite (RGB_OUT_PIN+1, limitLowColor(20, pColor[1]));
   //BLUE : as is
   analogWrite (RGB_OUT_PIN+2, pColor[2]);
}

int map2 (int pVal, int pFromLow, int pFromHigh, int pToLow, int pToHigh) {
   int vVal = map (pVal, pFromLow, pFromHigh, pToLow, pToHigh);
   
   if (vVal > pToHigh) { vVal = pToHigh; }
   if (vVal < pToLow) { vVal = pToLow; }
   
   return vVal;
}

void setDefaults(){
   int sensorValue =0;
   for (int i = 0; i < 3; i++){
      RGB_HIGHLOW[i][0] = 0;
      RGB_HIGHLOW[i][1] = 0;
   }

#ifdef DEBUG
   Serial.println ("Check white.");
#endif
   for (int j = 0; j < SAMPLECOUNT; j++) {
      for (int i = 0; i < 3; i++){
         digitalWrite (i+RGB_START_PIN, HIGH);
         delay(READ_LIGHT_DURATION);
         sensorValue = analogRead(A0);
         RGB_HIGHLOW[i][1] += sensorValue;
         digitalWrite (i+RGB_START_PIN, LOW); 
         delay(READ_LINE_COOLDOWN);     
      }
   }
   for (int i = 0; i < 3; i++){
      RGB_HIGHLOW[i][1] = RGB_HIGHLOW[i][1] / SAMPLECOUNT;
   }

   delay(3000);


#ifdef DEBUG
   Serial.println ("Check black.");
#endif
   for (int j = 0; j < SAMPLECOUNT; j++) {
      for (int i = 0; i < 3; i++){
         digitalWrite (i+5, HIGH);
         delay(200);
         sensorValue = analogRead(A0);
         RGB_HIGHLOW[i][0] += sensorValue;
         digitalWrite (i+5, LOW);  
         delay(50);         
      }
   }
   for (int i = 0; i < 3; i++){
      RGB_HIGHLOW[i][0] = RGB_HIGHLOW[i][0] / SAMPLECOUNT;
   }

#ifdef DEBUG
   Serial.println ("Done doing defaults.");
#endif
   showDefaults();

   delay(8000);
}

void showDefaults() {
#ifdef DEBUG
   Serial.print ("R [");
   Serial.print (RGB_HIGHLOW[0][0]);
   Serial.print (";");
   Serial.print (RGB_HIGHLOW[0][1]);
   Serial.print ("]  -  G [");
   Serial.print (RGB_HIGHLOW[1][0]);
   Serial.print (";");
   Serial.print (RGB_HIGHLOW[1][1]);
   Serial.print ("]  -  B [");   
   Serial.print (RGB_HIGHLOW[2][0]);
   Serial.print (";");
   Serial.print (RGB_HIGHLOW[2][1]);
   Serial.print ("]");
   Serial.println ("");
#endif
}

void storeDefaults(){
   int vAddr = EEPROM_DFLT_START;
   for (int i =0; i < 3; i++){
      for (int j = 0; j < 2; j++){
         int vValue = RGB_HIGHLOW[i][j];
         EEPROM.write(vAddr++, vValue);
         vValue = vValue >> 8;
         EEPROM.write(vAddr++, vValue);         
      }
   }
}


void loadDefaults(){
   int vAddr = EEPROM_DFLT_START;
   for (int i =0; i < 3; i++){
      for (int j = 0; j < 2; j++){
         int vValue = EEPROM.read(vAddr +1);
         vValue = vValue << 8;
         vValue = vValue + EEPROM.read(vAddr);
         RGB_HIGHLOW[i][j] = vValue;
         
         vAddr +=2;
      }
   }
   
   //not initialized, set default hardcoded
   if (RGB_HIGHLOW[0][0] == 0 || RGB_HIGHLOW[0][0] == 0xFFFF){
#ifdef DEBUG
   Serial.println ("No default in EEPROM : use hard coded values.");
#endif
      RGB_HIGHLOW[0][0] = 936;
      RGB_HIGHLOW[0][1] = 623;
      RGB_HIGHLOW[1][0] = 947;
      RGB_HIGHLOW[1][1] = 652;
      RGB_HIGHLOW[2][0] = 958;
      RGB_HIGHLOW[2][1] = 849;
   }
}
