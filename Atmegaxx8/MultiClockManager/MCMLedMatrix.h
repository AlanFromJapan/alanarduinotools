
#ifndef __MCMLedMatrix_h__
#define __MCMLedMatrix_h__

#include <util/delay.h>

//THE in memory matrix when using led matrix display
#define WCLK_MATRIX_SIZE 5
uint8_t WCLK_LED_MATRIX[WCLK_MATRIX_SIZE];

//#define WCLK_COL1LEFT (WCLK_MATRIX_SIZE * 2)

//duration in us of the illumination of the current led while drawing
#ifndef WCLK_POV_DURATION
	//if not overridden in includer file, set a value
	#define WCLK_POV_DURATION 200
#endif //WCLK_POV_DURATION

//ALL Reversed -> all leds off : PORTB.PORTD = 0bxxxxxx11.11100000
#define LEDS_ALL_OFF() 	{ PORTB |= 0x03; PORTD = 0xe0; }


//transpose the matrix (row <=> columns): used for legacy layouts
#define LED_MATRIX_TRANSPOSE	

/************************************************************************/
/* Reset matrix to empty values (blank)                                 */
/************************************************************************/
void resetLedMatrix(){
   //reset matrix
   for (uint8_t i=0; i < WCLK_MATRIX_SIZE; i++){
      WCLK_LED_MATRIX[i] = 0;
   }
}

/************************************************************************/
/* Initialize the pins for using led matrix                             */
/************************************************************************/
void setupLedMatrix() {
   resetLedMatrix();

	//	Set pins output mode
	
	//Rows : mix of PORTD and PORTB ...
	DDRB |= 0x03; //B0-B1
	DDRD |= 0xe0; //D5-D7

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


//simple permutation of 2 parameters
inline void _transposeColRow (uint8_t *col, uint8_t *row) { 
	uint8_t xxxx = *col; 
	*col = *row; 
	*row = xxxx; 
}

/************************************************************************/
/* Set a cell of the matrix to a given value                            */
/************************************************************************/
void setCell (uint8_t row, uint8_t col, uint8_t val){
#ifdef LED_MATRIX_TRANSPOSE	
	//cheap ugly trick, transposes row and column (too lazy to rewrite the drawLedMatrix)
	_transposeColRow(&col, &row);
#endif //LED_MATRIX_TRANSPOSE

   if (val){
      WCLK_LED_MATRIX[row] |= (1 << col);
   }
   else {
      WCLK_LED_MATRIX[row] &= ~(1 << col);
   }
}

/************************************************************************/
/* Toggle a cell of the matrix to a given value                            */
/************************************************************************/
void toggleCell (uint8_t row, uint8_t col){
#ifdef LED_MATRIX_TRANSPOSE
	//cheap ugly trick, transposes row and column (too lazy to rewrite the drawLedMatrix)
	_transposeColRow(&col, &row);
#endif //LED_MATRIX_TRANSPOSE
	
   WCLK_LED_MATRIX[row] = WCLK_LED_MATRIX[row] ^ (1 << col);
}

/************************************************************************/
/* Draws the in memory matrix to the led board.                         */
/* You will most likely rewrite this function unless you use direct wiring like is planned on the main board.*/
/************************************************************************/
void drawLedMatrix (){
	//ALL Reversed -> all leds off : PORTB.PORTD = 0bxxxxxx11.11100000 
	LEDS_ALL_OFF();
	
   //this does NOT run in constant time because we only turns led on, those off are already off so they are skipped.
   //side effect: the more there is led on, the slower it is to draw it.
   
   uint8_t r = 0;
   uint8_t rMask = 0;
   //Raw 1 (top)
   r=0;
   rMask = 0x01; //inverse logic (makes B1 = 0 => sink current)
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
   rMask = 0x02; //inverse logic (makes B0 = 0 => sink current)
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


#endif //__MCMLedMatrix_h__
