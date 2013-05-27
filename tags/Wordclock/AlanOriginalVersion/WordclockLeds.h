#include "Arduino.h"

#ifndef __WordclockLeds_h__
#define __WordclockLeds_h__

int WCLK_LED_MATRIX[5];
#define WCLK_MATRIX_SIZE 5
#define WCLK_ROWA 0
#define WCLK_COL1 9
#define WCLK_COL1LEFT 10

//duration in us of the illumination of the current led while drawing
#define WCLK_POV_DURATION 200


void resetLedMatrix(){
   //reset matrix
   for (int i=0; i < WCLK_MATRIX_SIZE; i++){
      WCLK_LED_MATRIX[i] = 0;
   }
}

void setupLedMatrix() {
   resetLedMatrix();

   for (int i=WCLK_COL1; i> WCLK_COL1 - WCLK_MATRIX_SIZE; i--){
      pinMode(i, OUTPUT);
   } 
   for (int i=WCLK_ROWA; i< WCLK_ROWA + 5; i++){
      pinMode(i, OUTPUT);
   } 
}

void setupLedMatrix2x1() {
   setupLedMatrix();
   
   for (int i=WCLK_COL1LEFT; i< WCLK_COL1LEFT + 5; i++){
      pinMode(i, OUTPUT);
   } 
}




void setCell (int row, int col, boolean val){
   if (val){
      WCLK_LED_MATRIX[row] = WCLK_LED_MATRIX[row] | (1 << col);
   }
   else {
      WCLK_LED_MATRIX[row] = WCLK_LED_MATRIX[row] & ~(1 << col);
   }
}

void toggleCell (int row, int col){
   WCLK_LED_MATRIX[row] = WCLK_LED_MATRIX[row] ^ (1 << col);
}

//Draws the in memory matrix to the led board.
//You will most likely rewrite this function unless you use direct wiring like is planned on the main board.
//Use this as a source of inspiration and make your own in case of custom wiring.
//If you want to gain some time, replace the digitalWrite by some pseudoC code.
void drawLedMatrix (){
   //COLUMNS are NEGATIVE side, so turn them off by putting them all HIGH
   //My bad : the pinds are reversed ordered but that was easier to wire...
   for (int i=WCLK_COL1; i> WCLK_COL1 - WCLK_MATRIX_SIZE; i--){
      digitalWrite(i, HIGH);
   } 

   //this does NOT run in constant time because we only turns led on, those off are already off so they are skipped.
   //side effect: the more there is led on, the slower it is to draw it.
   for (int r=0; r < WCLK_MATRIX_SIZE; r++){
      for (int c=0; c < WCLK_MATRIX_SIZE; c++){
         if (0 != (WCLK_LED_MATRIX[r] & (1 << c))){
            //turn the led on
            digitalWrite(r + WCLK_ROWA, HIGH);
            digitalWrite(WCLK_COL1- c, LOW); //minus because reverse order

            //POV
            delayMicroseconds(WCLK_POV_DURATION);

            //reverse the pins HIGH/LOW status to turn it off
            digitalWrite(r + WCLK_ROWA, LOW);
            digitalWrite(WCLK_COL1- c, HIGH); //minus because reverse order
         }
      }   
   }
}


void drawLedMatrix2x1 (){
   //COLUMNS are NEGATIVE side, so turn them off by putting them all HIGH
   for (int i=WCLK_COL1; i> WCLK_COL1 - WCLK_MATRIX_SIZE; i--){
      digitalWrite(i, HIGH);
   } 
   for (int i=WCLK_COL1LEFT; i< WCLK_COL1LEFT + WCLK_MATRIX_SIZE; i++){
      digitalWrite(i, HIGH);
   } 

   //this does NOT run in constant time because we only turns led on, those off are already off so they are skipped.
   //side effect: the more there is led on, the slower it is to draw it.
   for (int r=0; r < WCLK_MATRIX_SIZE; r++){
      //Left panel
      for (int c=0; c < 5; c++){
         if (0 != (WCLK_LED_MATRIX[r] & (1 << c))){
            //turn the led on
            digitalWrite(r + WCLK_ROWA, HIGH);
            digitalWrite(WCLK_COL1LEFT +4 -c, LOW);

            delayMicroseconds(WCLK_POV_DURATION);

            //reverse the pins HIGH/LOW status to turn it off
            digitalWrite(r + WCLK_ROWA, LOW);
            digitalWrite(WCLK_COL1LEFT +4 -c, HIGH);
         }
      }  

      //Right panel
      for (int c=5; c < 10; c++){
         if (0 != (WCLK_LED_MATRIX[r] & (1 << c))){
            //turn the led on
            digitalWrite(r + WCLK_ROWA, HIGH);
            digitalWrite(WCLK_COL1 -(c-5), LOW);

            delayMicroseconds(WCLK_POV_DURATION);

            //reverse the pins HIGH/LOW status to turn it off
            digitalWrite(r + WCLK_ROWA, LOW);
            digitalWrite(WCLK_COL1-(c-5) , HIGH);
         }
      }   


   }
}


#endif //__WordclockLeds_h__
