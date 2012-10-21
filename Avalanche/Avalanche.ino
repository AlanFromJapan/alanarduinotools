//
// http://kalshagar.wikispaces.com/Avalanche
// By AlanFromJapan 2012/10/16
// Feel free to reuse
//

/*
TLC5940 lib comment
 Basic Pin setup:
 ------------                                  ---u----
 ARDUINO   13|-> SCLK (pin 25)           OUT1 |1     28| OUT channel 0
 12|                           OUT2 |2     27|-> GND (VPRG)
 11|-> SIN (pin 26)            OUT3 |3     26|-> SIN (pin 11)
 10|-> BLANK (pin 23)          OUT4 |4     25|-> SCLK (pin 13)
 9|-> XLAT (pin 24)             .  |5     24|-> XLAT (pin 9)
 8|                             .  |6     23|-> BLANK (pin 10)
 7|                             .  |7     22|-> GND
 6|                             .  |8     21|-> VCC (+5V)
 5|                             .  |9     20|-> 2K Resistor -> GND
 4|                             .  |10    19|-> +5V (DCPRG)
 3|-> GSCLK (pin 18)            .  |11    18|-> GSCLK (pin 3)
 2|                             .  |12    17|-> SOUT
 1|                             .  |13    16|-> XERR
 0|                           OUT14|14    15| OUT channel 15
 ------------                                  --------
 
 -  Put the longer leg (anode) of the LEDs in the +5V and the shorter leg
 (cathode) in OUT(0-15).
 -  +5V from Arduino -> TLC pin 21 and 19     (VCC and DCPRG)
 -  GND from Arduino -> TLC pin 22 and 27     (GND and VPRG)
 -  digital 3        -> TLC pin 18            (GSCLK)
 -  digital 9        -> TLC pin 24            (XLAT)
 -  digital 10       -> TLC pin 23            (BLANK)
 -  digital 11       -> TLC pin 26            (SIN)
 -  digital 13       -> TLC pin 25            (SCLK)
 -  The 2K resistor between TLC pin 20 and GND will let ~20mA through each
 LED.  To be precise, it's I = 39.06 / R (in ohms).  This doesn't depend
 on the LED driving voltage.
 - (Optional): put a pull-up resistor (~10k) between +5V and BLANK so that
 all the LEDs will turn off when the Arduino is reset.
 
 If you are daisy-chaining more than one TLC, connect the SOUT of the first
 TLC to the SIN of the next.  All the other pins should just be connected
 together:
 BLANK on Arduino -> BLANK of TLC1 -> BLANK of TLC2 -> ...
 XLAT on Arduino  -> XLAT of TLC1  -> XLAT of TLC2  -> ...
 The one exception is that each TLC needs it's own resistor between pin 20
 and GND.
 
 This library uses the PWM output ability of digital pins 3, 9, 10, and 11.
 Do not use analogWrite(...) on these pins.
 
 This sketch does the Knight Rider strobe across a line of LEDs.
 
 Alex Leone <acleone ~AT~ gmail.com>, 2009-02-03 */

#include "Tlc5940.h"


//Pin connected to ST_CP of 74HC595
int latchPin = 18;
//Pin connected to SH_CP of 74HC595
int clockPin = 19;
////Pin connected to DS of 74HC595
int dataPin = 5;

void setup()
{
   /* Call Tlc.init() to setup the tlc.
    You can optionally pass an initial PWM value (0 - 4095) for all channels.*/
   Tlc.init();

   pinMode(latchPin, OUTPUT);
   pinMode(clockPin, OUTPUT);
   pinMode(dataPin, OUTPUT);

   //random seed on a unplugged pin
   randomSeed(analogRead(0));

}

void shiftOutX(uint8_t pR, uint8_t pG, uint8_t pB)
{
   //PD5 = data
   //PC5 = clock
   //PC4 = latch

   //digitalWrite(latchPin, LOW);
   PORTC &= 0b11101111; // ~PC4

   for (uint8_t j = 0; j < 3; j++){   
      uint8_t vVal = pB;
      switch (j){
         //case 0: vVal = pB; break;
         case 1: vVal = pG; break;
         case 2: vVal = pR; break;
      }
      
      for (uint8_t i = 0; i < 8; i++)  {
         //digitalWrite(dataPin, !!(val & (1 << (7 - i))));
         if ((vVal & (1 << i)) == 0){
            PORTD &= 0b11011111;
         }
         else {
            PORTD |= 0b00100000;         
         }

         //Clock up-down
         //digitalWrite(clockPin, HIGH);
         PORTC |= 0b00100000; // ~PC5
         delayMicroseconds(2);
         //digitalWrite(clockPin, LOW);  
         PORTC &= 0b11011111; // ~PC5          
      }
   }

   //digitalWrite(latchPin, HIGH);
   PORTC |= 0b00010000; // ~PC4
}

void writeRGB (int pR, int pG, int pB){
   
   shiftOutX(pR,pG,pB);
/*   
   // take the latchPin low so 
   // the LEDs don't change while you're sending in bits:
   digitalWrite(latchPin, LOW);

   //blue
   // shift out the bits:
   shiftOut(dataPin, clockPin, MSBFIRST, pB);  

   //green
   // shift out the bits:
   shiftOut(dataPin, clockPin, MSBFIRST, pG);

   //red
   // shift out the bits:
   shiftOut(dataPin, clockPin, MSBFIRST, pR);

   //take the latch pin high so the LEDs will light up:
   digitalWrite(latchPin, HIGH);
*/
}

/***************************************************************************************/
/***************************************************************************************/
void allAtTheSameTime()
{
   // take the latchPin low so 
   // the LEDs don't change while you're sending in bits:
   digitalWrite(latchPin, LOW);

   //blue
   // shift out the bits:
   shiftOut(dataPin, clockPin, MSBFIRST, random(256));  

   //green
   // shift out the bits:
   shiftOut(dataPin, clockPin, MSBFIRST, random(256));

   //red
   // shift out the bits:
   shiftOut(dataPin, clockPin, MSBFIRST, random(256));

   //and ground 
   int vGround = random(256);
   while (vGround == 0){
      vGround = random(256);
   }

   //take the latch pin high so the LEDs will light up:
   digitalWrite(latchPin, HIGH);


   int vStep = 25 + random(100);
   int vDelay = 25 + random(150);
   int vMax = 3000; //4095

   //light up
   for (int i =0; i < vMax; i += vStep){
      for (int j =1; j <= 8; j++){
         if ((vGround & (j-1)) != 0){
            Tlc.set(j, i);
         }
      }
      Tlc.update();
      delay (vDelay);
   }

   //light down
   for (int i =vMax; i > 0; i -= vStep){
      for (int j =1; j <= 8; j++){
         if ((vGround & (j-1)) != 0){
            Tlc.set(j, i);
         }
      }
      Tlc.update();
      delay (vDelay);
   }

   //Zero
   for (int j =1; j <= 8; j++){
      Tlc.set(j, 0);
   }
   Tlc.update();

   //wait a little before next round
   delay (500 * random(10) + 1000);
}


/***************************************************************************************/
/***************************************************************************************/
void colorPerColor()
{
   int vR = random(256);
   int vG = random(256);
   int vB = random(256);

   //and ground 
   int vGround = random(256);
   while (vGround == 0){
      vGround = random(256);
   }

   int vStep = 25 + random(50);
   int vDelay = 25 + random(50);
   int vMax = 3000; //4095

   int vRout, vGout, vBout;
   vRout = 0; 
   vRout = 0; 
   vBout = 0;   

   uint8_t vColorFlag = 0;

   while (vColorFlag != 0x07){
      //choose one color
      int vBit = random(3);
      while ((vColorFlag & (1 << vBit)) != 0){
         vBit = random(3);
      }

      vColorFlag |= (1 << vBit);

      switch(vBit){
      case 0: 
         vRout = vR; 
         break;
      case 1: 
         vGout = vG; 
         break;
      case 2: 
         vBout = vB; 
         break;
      }

      // take the latchPin low so 
      // the LEDs don't change while you're sending in bits:
      digitalWrite(latchPin, LOW);

      //blue
      // shift out the bits:
      shiftOut(dataPin, clockPin, MSBFIRST, vBout);  

      //green
      // shift out the bits:
      shiftOut(dataPin, clockPin, MSBFIRST, vGout);

      //red
      // shift out the bits:
      shiftOut(dataPin, clockPin, MSBFIRST, vRout);

      //take the latch pin high so the LEDs will light up:
      digitalWrite(latchPin, HIGH);



      //light up
      for (int i =0; i < vMax; i += vStep){
         for (int j =1; j <= 8; j++){
            if ((vGround & (j-1)) != 0){
               Tlc.set(j, i);
            }
         }
         Tlc.update();
         delay (vDelay);
      }

   }
   //light down
   for (int i =vMax; i > 0; i -= vStep){
      for (int j =1; j <= 8; j++){
         if ((vGround & (j-1)) != 0){
            Tlc.set(j, i);
         }
      }
      Tlc.update();
      delay (vDelay);
   }

   //Zero
   for (int j =1; j <= 8; j++){
      Tlc.set(j, 0);
   }
   Tlc.update();

   //wait a little before next round
   delay (500 * random(10) + 1000);
}


/***************************************************************************************/
/***************************************************************************************/
void aLaMatrix()
{
   int vR = 1 << random(8);
   int vG = 1 << random(8);
   int vB = 1 << random(8);

   //and ground 
   int vGround = 0;

   int vStep = 100 + random(150);
   int vDelay = 10 + random(25);
   int vMax = 3000; //4095

   int vRout, vGout, vBout;
   vRout = 0; 
   vGout = 0; 
   vBout = 0;   

   uint8_t vColorFlag = 0;

   while (vColorFlag != 0x07){
      //choose one color
      int vBit = random(3);
      while ((vColorFlag & (1 << vBit)) != 0){
         vBit = random(3);
      }

      vColorFlag |= (1 << vBit);

      switch(vBit){
      case 0: 
         vRout = vR; 
         break;
      case 1: 
         vGout = vG; 
         break;
      case 2: 
         vBout = vB; 
         break;
      }

      // take the latchPin low so 
      // the LEDs don't change while you're sending in bits:
      digitalWrite(latchPin, LOW);

      //blue
      // shift out the bits:
      shiftOut(dataPin, clockPin, MSBFIRST, vBout);  

      //green
      // shift out the bits:
      shiftOut(dataPin, clockPin, MSBFIRST, vGout);

      //red
      // shift out the bits:
      shiftOut(dataPin, clockPin, MSBFIRST, vRout);

      //take the latch pin high so the LEDs will light up:
      digitalWrite(latchPin, HIGH);



      //light up
      for (int j =1; j <= 8; j++){
         for (int i =0; i < vMax; i += vStep){
            Tlc.set(j, i);

            Tlc.update();
            delay (vDelay);
         }
      }

   }
   //light down
   for (int i =vMax; i > 0; i -= vStep){
      for (int j =1; j <= 8; j++){
         Tlc.set(j, i);

      }
      Tlc.update();
      delay (vDelay);
   }

   //Zero
   for (int j =1; j <= 8; j++){
      Tlc.set(j, 0);
   }
   Tlc.update();

   //wait a little before next round
   delay (500 * random(10) + 1000);
}



/***************************************************************************************/
/***************************************************************************************/
void oneCell()
{
   int vR = 0x02;
   int vG = 0x02;
   int vB = 0x00;

   //and ground 
   int vGround = 0;

   int vStep = 10 + random(15);
   int vDelay = 2;
   int vMax = 3000; //4095

   int vRout, vGout, vBout;
   vRout = vR; 
   vGout = vG; 
   vBout = 0;   


   //light up
   for (int j =1; j <= 8; j++){
      for (int i =0; i < vMax; i += vStep){
         writeRGB(0, 0x40, 0);
         Tlc.set(j, i);
         Tlc.update();
         delay (vDelay);


         writeRGB(0x40, 0, 0);
         Tlc.set(j, i);
         Tlc.update();
         delay (vDelay);


         writeRGB(0, 0, 0x02);
         Tlc.set(j, i);
         Tlc.update();
         delay (vDelay);
      }
   }




   //light down
   for (int i =vMax; i > 0; i -= vStep){
      for (int j =1; j <= 8; j++){
         Tlc.set(j, i);

      }
      Tlc.update();
      delay (vDelay);
   }

   //Zero
   for (int j =1; j <= 8; j++){
      Tlc.set(j, 0);
   }
   Tlc.update();

   //wait a little before next round
   delay (1000);
}

/***************************************************************************************/
/***************************************************************************************/
void loop()
{
   oneCell();
   /*
   int vPattern = random (2);
    switch(vPattern){
    case 0:
    allAtTheSameTime();
    break;
    case 1:
    colorPerColor();
    break;
    
    }
    */

}




















