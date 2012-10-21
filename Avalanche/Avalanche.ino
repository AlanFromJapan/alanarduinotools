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

void loop()
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














