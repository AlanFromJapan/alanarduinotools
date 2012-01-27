/*
 * KitchenTimer
 * ------------
 * A very unreasonable replacement for a 2$ kitchen timer.
 * http://kalshagar.wikispaces.com/KitchenTimer
 * By AlanFromJapan, Jan2012.
 * (ArduinoIDE 0021)
 */

/*
 * Based on GLCD lib, example BigNums
 */
#include "glcd.h"

/*
 * individual fonts don't have to be included
 * if you include the allFonts.h header
 */
#include "fonts/fixednums15x31.h"
#include "fonts/SystemFont5x7.h"

#define PIN_BUTTON_A (1024/3)
#define PIN_BUTTON_B (1024/2)
#define PIN_BUTTON_START 1024
#define PIN_MUSIC 13
#define PIN_LED_A 2
#define PIN_LED_B 3

#define BUTTON_READ_PAUSE 300

//duration associated to buttons (in seconds)
#define DURATION_A (60 * 5)
#define DURATION_B (15)

unsigned long startTime;
unsigned long timerDuration;

void setup() {
   delay(10);//let millis() startup
   setupGameOver();
   //showGameOver();
   setupGLCD();

   //Start by a splash screen
   showSplashscreen();

   //read inputs
   readTimeAndStart();
   //timerDuration = 1000 * 5;

   //Save the start time
   setupForShowTime();

   //... and now the loop()
}

//Main loop: just in charge of showing time passing by and triggering the gameover
void loop()
{
   unsigned long timeSpent, remainingTime;
   timeSpent = millis() - startTime;
   //don't use max because value will circle and become very big
   remainingTime = (timerDuration > timeSpent ? timerDuration - timeSpent : 0);

   showTime(remainingTime);
   //delay to avoid unnecessary refresh
   delay(500);

   if (remainingTime == 0){
      //Show the end of timer lights and sounds
      showGameOver();
   }
}

void setupGameOver(){
   pinMode(PIN_MUSIC, OUTPUT);
   pinMode(PIN_LED_A, OUTPUT);
   pinMode(PIN_LED_B, OUTPUT);
   digitalWrite (PIN_MUSIC, LOW);
}

//Light and sound for the fun
void showGameOver(){
   unsigned long vNextSound = millis()-1;

   while (true){
      //do some light bling bling here ...

      //sound every once in a while : you have to trigger it (optocoupler) and cut it right after
      //this mimics the click on the button. The sound keyholder does the rest.
      if (millis() > vNextSound){
         //optocoupler on -> simulate click on button -> go for the show
         digitalWrite (PIN_MUSIC, HIGH);
         delay(100);
         digitalWrite (PIN_MUSIC, LOW);

         //next click in 10 sec
         vNextSound = millis() + 1000 * 6; //every 6 sec

         //it's time !
         GLCD.SelectFont(SystemFont5x7);
         GLCD.CursorToXY(random(0,80),random(0,64-7));
         GLCD.Puts("It's time!");

         //blingbling
         digitalWrite(PIN_LED_A, HIGH);
         digitalWrite(PIN_LED_B, HIGH);
         delay(50);
         for(int i = 0; i < 4; i++){
            digitalWrite(PIN_LED_A, LOW);
            digitalWrite(PIN_LED_B, HIGH);
            delay(50);
            digitalWrite(PIN_LED_A, HIGH);
            digitalWrite(PIN_LED_B, LOW);
            delay(50);
         }
         digitalWrite(PIN_LED_A, LOW);
         digitalWrite(PIN_LED_B, LOW);

      }
   }
}


//Setup buttons on input and read duration
void readTimeAndStart(){

   boolean vStart = false;
   timerDuration = 0;
   while (!vStart){
      int sensorValue = analogRead(A5);

      //Read button A
      if (readButton(sensorValue, PIN_BUTTON_A)){
         //Add a given amount of time
         timerDuration += (unsigned long)(1000) * (unsigned long)(DURATION_A);
         //and show
         showTime(timerDuration);

         //don't go too fast (read x times input)
         delay(BUTTON_READ_PAUSE);
      }       


      //Read button B
      if (readButton(sensorValue, PIN_BUTTON_B)){
         //Add a given amount of time
         timerDuration += (unsigned long)(1000) * (unsigned long)(DURATION_B);
         //and show
         showTime(timerDuration);

         //don't go too fast (read x times input)
         delay(BUTTON_READ_PAUSE);
      }      


      //at last, read the start button
      if (readButton(sensorValue, PIN_BUTTON_START)){
         //Let's go !
         vStart = true; 
      }


   }
}


boolean readButton (int pSensorValue, int pExpected) {
   return pSensorValue > pExpected - 10
      &&  pSensorValue < pExpected + 10;  
}


//Setup for Graphic LCD
void setupGLCD(){
   GLCD.Init(NON_INVERTED);
}

//A nice splash screen while waiting for start
void showSplashscreen(){
   //GLCD.ClearScreen();
   GLCD.ClearArea();
   GLCD.FillRect(0,0,128,64, false);

   //Small font : buttons
   GLCD.SelectFont(SystemFont5x7);
   GLCD.CursorToXY(0,0);
   GLCD.Puts("+5min    +15s     Go!");

   //Medium font : logo
   GLCD.SelectFont(SystemFont5x7);
   //   GLCD.CursorToXY(0,(64 - 32)/2); GLCD.Puts("KitchenTimer"); 
   GLCD.CursorToXY(0,20); 
   GLCD.Puts("    KitchenTimer"); 



   //Small font : ego boost
   GLCD.SelectFont(SystemFont5x7);
   //GLCD.CursorToXY(0,64-7); GLCD.Puts(" http://goo.gl/xo1QV");
   GLCD.CursorToXY(0,64-8*3); 
   GLCD.Puts("http://kalshagar.wikispaces.com/KitchenTimer");


   delay(2000);
}

//Setup the screen before showing time
void setupForShowTime(){
   //GLCD.SetFontColor(WHITE); // set font color 
   startTime = millis();
   delay(10);
}

//Show time on the screen
void showTime(unsigned long pDuration) {
   int hr, min, sec;

   GLCD.ClearArea();

   hr = ((pDuration / 1000) / 60) / 60;
   min = ((pDuration / 1000) / 60) % 60;
   sec = (pDuration / 1000) % 60;

   //center vertical
   GLCD.CursorToXY(0,32/2); // column & row is relative to text area
   //big font
   GLCD.SelectFont(fixednums15x31);
   /*
      * use a formatting string that is only in FLASH/(program memory) and *not* in RAM.
    */
   GLCD.Printf_P(PSTR("%02d:%02d.%02d"), hr, min, sec);
}




