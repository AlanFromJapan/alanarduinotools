/*
* Rapide7seg_Test.c
*
* Created: 2012/12/09 23:52:15
*  Author: Alan
*
*  This thing will continuously display the whatever value it wants to (for retinal persistence) as main loop, and regularly read the buttons pressed via timer interrupt.
*/

//#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define DIGIT_DOT	0b01111111
#define DIGIT_0		0b11000000
#define DIGIT_1		0b11111001
#define DIGIT_2		0b10100100
#define DIGIT_3		0b10110000
#define DIGIT_4		0b10011001
#define DIGIT_5		0b10010010
#define DIGIT_6		0b10000010
#define DIGIT_7		0b11111000
#define DIGIT_8		0b10000000
#define DIGIT_9		0b10010000
#define DIGIT_A		0b10001000
#define DIGIT_B		0b10000011
#define DIGIT_C		0b11000110
#define DIGIT_D		0b10100001
#define DIGIT_E		0b10000110
#define DIGIT_F		0b10001110

uint8_t DIGITS[] = {
	DIGIT_0,
	DIGIT_1,
	DIGIT_2,
	DIGIT_3,
	DIGIT_4,
	DIGIT_5,
	DIGIT_6,
	DIGIT_7,
	DIGIT_8,
	DIGIT_9,
	DIGIT_A,
	DIGIT_B,
	DIGIT_C,
	DIGIT_D,
	DIGIT_E,
	DIGIT_F
};

//volatile since will change in an interrupt
volatile uint16_t theValue = 0;

//one value, one digit, left to right
uint8_t mDisplayTab[4];

const uint8_t KEYS_LOOKUP[] = {7, 8, 9, 4, 5, 6, 1, 2, 3, 'C', 0, '*'};


#define POV_DURATION_MS 	5
//Shows the in-memory table of 4 7segs
//Says from which to start and till which to go (0= leftmost, 3= rightmost)
void showDisplayTab(uint8_t pFromLeft, uint8_t pToRight){
	if (pFromLeft <= 0 && pToRight >= 0){
		PORTD = mDisplayTab[0];
		PORTC = 0x01;
		_delay_ms(POV_DURATION_MS);
	}

	if (pFromLeft <= 1 && pToRight >= 1){
		PORTD = mDisplayTab[1];
		PORTC = 0x02;
		_delay_ms(POV_DURATION_MS);
	}
	
	if (pFromLeft <= 2 && pToRight >= 2){
		PORTD = mDisplayTab[2];
		PORTC = 0x04;
		_delay_ms(POV_DURATION_MS);
	}

	if (pFromLeft <= 3 && pToRight >= 3){
		PORTD = mDisplayTab[3];
		PORTC = 0x08;
		_delay_ms(POV_DURATION_MS);
	}
}


void showNumber(uint16_t pNumber, uint8_t pFromLeft, uint8_t pToRight){
	//display on the 3 leftmost digits
	mDisplayTab[2] = DIGITS[pNumber % (uint16_t)10];
	mDisplayTab[1] = DIGITS[(pNumber / (uint16_t)10) % (uint16_t)10];
	mDisplayTab[0] = DIGITS[(pNumber / (uint16_t)100) % (uint16_t)10];
			
	showDisplayTab(pFromLeft,pToRight);
}


/*
 * Returns the pressed button
 *   7  8  9
 *   4  5  6
 *   1  2  3
 *   C  0  .
 *
 *
 */
inline uint8_t readButton() {

	for (uint8_t col = 0; col < 3; col++){
		// left most column is PB4 -> PB3 -> PB2
		PORTB = (~(1 << (4 - col))) | 0xC3;

		//in case to stabilize output?
		_delay_loop_1(16);

		// keep the rows only from what you read 0b11000011
		uint8_t rows = PINB & 0xC3;

		//turn off
		PORTB = 0xff; //0xC3;

		if (rows != 0){
			//something pressed
			uint8_t rowid = 0;
			if ((rows & 0x02) == 0){
				// 7 8 9
				rowid = 0;
			}
//			else if ((rows & 0x01) == 0) {
//				// 4 5 6
//				rowid = 1;
//			}
//			else if ((rows & 0x40) == 0) {
//				// 1 2 3
//				rowid = 2;
//			}
//			else {
//				// C 0 *
//				rowid = 3;
//			}

			return KEYS_LOOKUP[rowid * 3 + col];
		}
	}

	//nothing pressed
	return 0xff;
}


/*
 * The interrupt of timer overflow to trigger reading of keys status
 */
ISR(TIMER1_OVF_vect){
	if (readButton() != 0xff) {
		theValue += 1;
	}

}


/*
 * Prepares the registers and other init
 */
inline void setup7segments(){
	//factory settings is to divide internal clock 8MHz by 8.
	//don't, and just run at 8 MHz (set the clock divider to 1 so no effect)
	CLKPR = (1<<CLKPCE);
	CLKPR = 0; // Divide by 1
	
	//Driver of the common anode is from PC0 (1st 7seg) to PC3 (4th 7seg)
	//Lower 4 go output
	DDRC = 0x0F;
	//all segments on
	PORTC = 0xFF;
	
	//the 7seg legs are driven by pins PIND
	//all pins are output
	DDRD = 0xFF;

}


/*
 * Init for they key matrix
 */
inline void setupKeymatrix(){
	//PortB is used in scanning matrix mode : 4 rows of 3 buttons
	// Cols:      PB4 PB3 PB2
	// Rows: PB1   7   8   9
	//       PB0   4   5   6
	//       PB6   1   2   3
	//       PB7   C   0   *

	//Cols are output
	DDRB = (1 << PORTB4) | (1 << PORTB3) | (1 << PORTB2);

	//just make sure pullups are NOT disabled
	MCUCR |= (0 << PUD);

	//all low EXCEPT the input to become pulled up
	PORTB = 0xC3;
}


/*
 * Setup the TIMER1 interrupt (16bits) as overflow to know when to read the keyboard status
 */
inline void setupInterrupts(){

	//Timer1 is 16 bit overflow timer (65535)
	TCCR1A = 0x00;
	TCCR1B = (1<<CS11) ;  // Timer mode with 8 prescler
	TIMSK1 = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
	sei();        // Enable global interrupts by setting global interrupt enable bit in SREG
}


/*****************************************************************************************************
 * MAIN
 *****************************************************************************************************/
int main(void)
{
	setup7segments();
	
	setupKeymatrix();

	setupInterrupts();

	//main loop, just show number, that's it
	while(1)
	{
		showNumber(theValue, 0, 2);
	}
}

