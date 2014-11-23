
#ifndef __MCMLedMatrix_h__
#define __MCMLedMatrix_h__

#include <util/delay.h>

#define WCLK_MATRIX_SIZE 5
uint8_t WCLK_LED_MATRIX[WCLK_MATRIX_SIZE];

#define WCLK_COL1LEFT (WCLK_MATRIX_SIZE * 2)

//duration in us of the illumination of the current led while drawing
#define WCLK_POV_DURATION 200

//ALL Reversed -> all leds off : PORTB.PORTD = 0bxxxxxx11.11100000
#define LEDS_ALL_OFF() 	{ PORTB |= 0x03; PORTD = 0xe0; }
	

void resetLedMatrix(){
   //reset matrix
   for (uint8_t i=0; i < WCLK_MATRIX_SIZE; i++){
      WCLK_LED_MATRIX[i] = 0;
   }
}

void setupLedMatrix() {
   resetLedMatrix();

	//	Set pins output mode
	
	//Rows : mix of PORTD and PORTB ...
	DDRB |= 0x03; //B0-B1
	DDRD |= 0xD0; //D5-D7

	//Columns : simple, it's D0-D4
	DDRD |= 0x1f;
	
	LEDS_ALL_OFF();
}

void setupLedMatrix2x1() {
   setupLedMatrix();

	//	Set pins output mode
	// Extra columns are Arduino 10-14 => B2-B5 + C0   
	DDRB |= 0x3c;
	DDRC |= 0x01;
}




void setCell (uint8_t row, uint8_t col, uint8_t val){
   if (val){
      WCLK_LED_MATRIX[row] |= (1 << col);
   }
   else {
      WCLK_LED_MATRIX[row] &= ~(1 << col);
   }
}

void toggleCell (uint8_t row, uint8_t col){
   WCLK_LED_MATRIX[row] = WCLK_LED_MATRIX[row] ^ (1 << col);
}

//Draws the in memory matrix to the led board.
//You will most likely rewrite this function unless you use direct wiring like is planned on the main board.
//Use this as a source of inspiration and make your own in case of custom wiring.
//If you want to gain some time, replace the digitalWrite by some pseudoC code.
void drawLedMatrix (){
	//ALL Reversed -> all leds off : PORTB.PORTD = 0bxxxxxx11.11100000 
	LEDS_ALL_OFF();
	
   //this does NOT run in constant time because we only turns led on, those off are already off so they are skipped.
   //side effect: the more there is led on, the slower it is to draw it.
/*   for (uint8_t r=0; r < WCLK_MATRIX_SIZE; r++){
      for (uint8_t c=0; c < WCLK_MATRIX_SIZE; c++){
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
   }*/
   
   uint8_t r = 0;
   uint8_t rMask = 0;
   //Raw 1 (top)
   r=0;
   rMask = 0x01;
   for (uint8_t c=0; c < WCLK_MATRIX_SIZE; c++){
	   if (0 != (WCLK_LED_MATRIX[r] & (1 << c))){
		   //turn the led on
		   PORTB &= (0xfc | rMask); 
			PORTD |= (1 << c);
			
		   //POV
		   _delay_us(WCLK_POV_DURATION); // delayMicroseconds(WCLK_POV_DURATION);

			//all off
			LEDS_ALL_OFF();
	   }
   }
   

   r=1;
   rMask = 0x02;
   for (uint8_t c=0; c < WCLK_MATRIX_SIZE; c++){
	   if (0 != (WCLK_LED_MATRIX[r] & (1 << c))){
		   //turn the led on
		   PORTB &= (0xfc | rMask);
		   PORTD |= (1 << c);

		   //POV
		   _delay_us(WCLK_POV_DURATION); // delayMicroseconds(WCLK_POV_DURATION);

			//all off
			LEDS_ALL_OFF();
	   }
   }
   

   r=2;
   rMask = 0x60; 
   for (uint8_t c=0; c < WCLK_MATRIX_SIZE; c++){
	   if (0 != (WCLK_LED_MATRIX[r] & (1 << c))){
		   //turn the led on
		   PORTD &= rMask;
		   PORTD |= (1 << c);

		   //POV
		   _delay_us(WCLK_POV_DURATION); // delayMicroseconds(WCLK_POV_DURATION);

			//all off
			LEDS_ALL_OFF();
	   }
   }


   r=3;
   rMask = 0xa0;
   for (uint8_t c=0; c < WCLK_MATRIX_SIZE; c++){
	   if (0 != (WCLK_LED_MATRIX[r] & (1 << c))){
		   //turn the led on
		   PORTD &= rMask;
		   PORTD |= (1 << c);

		   //POV
		   _delay_us(WCLK_POV_DURATION); // delayMicroseconds(WCLK_POV_DURATION);

			//all off
			LEDS_ALL_OFF();
	   }
   }


   r=4;
   rMask = 0xc0;
   for (uint8_t c=0; c < WCLK_MATRIX_SIZE; c++){
	   if (0 != (WCLK_LED_MATRIX[r] & (1 << c))){
		   //turn the led on
		   PORTD &= rMask;
		   PORTD |= (1 << c);

		   //POV
		   _delay_us(WCLK_POV_DURATION); // delayMicroseconds(WCLK_POV_DURATION);

			//all off
			LEDS_ALL_OFF();
	   }
   }


}

/*
void drawLedMatrix2x1 (){
   //COLUMNS are NEGATIVE side, so turn them off by putting them all HIGH
   for (uint8_t i=WCLK_COL1; i> WCLK_COL1 - WCLK_MATRIX_SIZE; i--){
      digitalWrite(i, HIGH);
   } 
   for (uint8_t i=WCLK_COL1LEFT; i< WCLK_COL1LEFT + WCLK_MATRIX_SIZE; i++){
      digitalWrite(i, HIGH);
   } 

   //this does NOT run in constant time because we only turns led on, those off are already off so they are skipped.
   //side effect: the more there is led on, the slower it is to draw it.
   for (uint8_t r=0; r < WCLK_MATRIX_SIZE; r++){
      //Left panel
      for (uint8_t c=0; c < WCLK_MATRIX_SIZE; c++){
         if (0 != (WCLK_LED_MATRIX[r] & (1 << c))){
            //turn the led on
            digitalWrite(r + WCLK_ROWA, HIGH);
            digitalWrite(WCLK_COL1LEFT +(WCLK_MATRIX_SIZE -1) -c, LOW);

            delayMicroseconds(WCLK_POV_DURATION);

            //reverse the pins HIGH/LOW status to turn it off
            digitalWrite(r + WCLK_ROWA, LOW);
            digitalWrite(WCLK_COL1LEFT +(WCLK_MATRIX_SIZE -1) -c, HIGH);
         }
      }  

      //Right panel
      for (uint8_t c=WCLK_MATRIX_SIZE; c < (2 * WCLK_MATRIX_SIZE); c++){
         if (0 != (WCLK_LED_MATRIX[r] & (1 << c))){
            //turn the led on
            digitalWrite(r + WCLK_ROWA, HIGH);
            digitalWrite(WCLK_COL1 -(c-WCLK_MATRIX_SIZE), LOW);

            delayMicroseconds(WCLK_POV_DURATION);

            //reverse the pins HIGH/LOW status to turn it off
            digitalWrite(r + WCLK_ROWA, LOW);
            digitalWrite(WCLK_COL1-(c-WCLK_MATRIX_SIZE) , HIGH);
         }
      }   


   }
}
*/

#endif //__MCMLedMatrix_h__
