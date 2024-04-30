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
#include <avr/cpufunc.h> //for _NOP()


#define DIGIT_DOT		0b01111111
#define DIGIT_0			0b11000000
#define DIGIT_1			0b11111001
#define DIGIT_2			0b10100100
#define DIGIT_3			0b10110000
#define DIGIT_4			0b10011001
#define DIGIT_5			0b10010010
#define DIGIT_6			0b10000010
#define DIGIT_7			0b11111000
#define DIGIT_8			0b10000000
#define DIGIT_9			0b10010000
#define DIGIT_A			0b10001000
#define DIGIT_B			0b10000011
#define DIGIT_C			0b11000110
#define DIGIT_D			0b10100001
#define DIGIT_E			0b10000110
#define DIGIT_F			0b10001110
#define DIGIT_NONE		0b11111111
#define DIGIT_HYPHEN	0b10111111

#define DIGIT_MULTIPLICATOR_MEGA	0b11110110
#define DIGIT_MULTIPLICATOR_KILO 	0b11110111
#define DIGIT_MULTIPLICATOR_NONE 	0b11111111


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
volatile float theValue = 0;
volatile uint8_t theValueNthDigit = 0;

//show number in which base (base <= 16)
#define DIGIT_SHOW_BASE		10

#define MODE_RESISTOR	0
#define MODE_TEST		1
volatile uint8_t mode = MODE_RESISTOR;

//one value, one digit, left to right
uint8_t mDisplayTab[4];

//Columns are (1 << PORTB4) | (1 << PORTB3) | (1 << PORTB2)
#define KEYMX_COLUMNS	0x1C
//Rows are PB1,0,6,7
#define KEYMX_ROWS		0xC3
//easy mapping to pressed key
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


/**
 * Turn off the display
 */
void displayOff(){
	PORTD = DIGIT_NONE;
	PORTC = 0x00;
}

/**
 * Shows a number from pFromLeft display (start at 0) to pFromRight (max 3) in base (where base <=16)
 */
void showNumber(float pNumber, uint8_t pFromLeft, uint8_t pToRight, uint16_t base){
	float multiplicator = 1.0;
	//fix these dividers to make it work with other than base 10 (left for you. future me)
	if (theValue >= 1000000.0){
		multiplicator = 1000000.0;
		mDisplayTab[3] = DIGIT_MULTIPLICATOR_MEGA;
	}
	else if (theValue >= 1000.0){
		multiplicator = 1000.0;
		mDisplayTab[3] = DIGIT_MULTIPLICATOR_KILO;
	}
	else {
		mDisplayTab[3] = DIGIT_MULTIPLICATOR_NONE;
	}

	//keep only 3 relevant digit
	float adjustedValue = theValue / multiplicator;

	//if decimal is != 0 need to shift left the result
	uint8_t decimal = ((uint16_t)(adjustedValue * (float)base)) % base;

	if (decimal == 0){
		//no decimal

		uint16_t v = (uint16_t)adjustedValue;
		//display on the 3 leftmost digits
		mDisplayTab[2] = DIGITS[v % base];


		//middle digit or nothing
		v = v / base;
		if (v > 0) {
			mDisplayTab[1] = DIGITS[v % base];
		}
		else {
			mDisplayTab[1] = DIGIT_NONE;
		}

		//leftmost digit or nothing
		v = v / base;
		if (v > 0) {
			mDisplayTab[0] = DIGITS[v % base];
		}
		else {
			mDisplayTab[0] = DIGIT_NONE;
		}
	}
	else {
		//there IS a decimal so shiftleft, add a dot and the one decimal

		//1 decimal (the max for resistor anyway)
		uint16_t v = (uint16_t)(adjustedValue * (float)base);

		//display on the 3 leftmost digits
		mDisplayTab[2] = DIGITS[v % base];

		//there's always a 2nd digit when there's a dot (even if it's a zero)
		v = v / base;
		mDisplayTab[1] = DIGITS[v % base] & DIGIT_DOT; //<== Dot if there is is ALWAYS on 2nd digit

		//leftmost digit or nothing
		v = v / base;
		if (v > 0) {
			mDisplayTab[0] = DIGITS[v % base];
		}
		else {
			mDisplayTab[0] = DIGIT_NONE;
		}
	}



	showDisplayTab(0,3);
}


/*
 * Returns the pressed button (or 0xff if nothing pressed)
	PortB is used in scanning matrix mode : 4 rows of 3 buttons
	 Cols:      PB4 PB3 PB2
	 Rows: PB1   7   8   9
		   PB0   4   5   6
		   PB6   1   2   3
		   PB7   C   0   *
 */
inline uint8_t readButton() {

	for (uint8_t col = 0; col < 3; col++){
		// left most column is PB4 -> PB3 -> PB2 (and keep the Rows pulled up), pull *DOWN* the Column you want to check
		PORTB = KEYMX_ROWS | ~(1 << (4 - col));

		//in case to stabilize output (need one as per the doc, make it 2 because I'm generous like that)
		_NOP();
		_NOP();

		// keep the rows only from what you read 0b11000011
		uint8_t rowsStatus = PINB & KEYMX_ROWS;

		//turn off (only keep the rows pulled up)
		PORTB = KEYMX_ROWS;

		if (rowsStatus != KEYMX_ROWS){
			//something pressed, consider only 1 key (multipress NOT supported)

			uint8_t rowid = 255;
			if ((rowsStatus & 0x02) == 0){
				// 7 8 9
				rowid = 0;
			}
			else if ((rowsStatus & 0x01) == 0) {
				// 4 5 6
				rowid = 1;
			}
			else if ((rowsStatus & 0x40) == 0) {
				// 1 2 3
				rowid = 2;
			}
			else if ((rowsStatus & 0x80) == 0){
				// C 0 *
				rowid = 3;
			}

			return KEYS_LOOKUP[rowid * 3 + col];
		}
	}

	//nothing pressed
	return 0xff;
}


/**
 * Executes the calculation for resistor value
 */
inline void resistorCalculation(const uint8_t button){
	switch (theValueNthDigit){
		case 0:
			//first digit
			theValue = (float)button;
			break;
		case 1:
			//2nd digit
			theValue = theValue * 10.0 + (float)button;
			break;
		case 2:
			//3rd digit (power!)

			//not using math POW() since it creates strange roundings with float
			//theValue = theValue * pow(10, button);

			//homegrown "* 10 ^ button"
			for (uint8_t po = 1; po <= button; po++){
				theValue = theValue * 10.0;
			}

			break;
	}

	theValueNthDigit++;
}


/*
 * The interrupt of timer overflow to trigger reading of keys status
 */
ISR(TIMER1_OVF_vect){
	uint8_t button = readButton();

	if (readButton() != 0xff) {
		//display off
		displayOff();

		//something pressed
		switch (button){
			case '*':
				//toggle mode
				mode = (mode == MODE_RESISTOR? MODE_TEST: MODE_RESISTOR);
				//break; //no break! Do a reset!
			case 'C':
				//reset
				theValue = 0;
				theValueNthDigit = 0;
				break;
			default:
				//other numbers
				if (mode == MODE_TEST){
					//shows pressed key
					theValue = button;
				}
				else if (mode == MODE_RESISTOR){
					//calculate the value

					//Dummy test
					//theValue += (float)button + 0.1;

					resistorCalculation(button);
				}

		}

		//debouncing on the cheap
		_delay_ms(200);
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

	//Columns are output (and rows input)
	DDRB = KEYMX_COLUMNS;

	//just make sure pullups are NOT disabled (set PUD in MCUCR to 0)
	MCUCR &= ~(1 << PUD);

	//Outputs (columns) are low and Inputs (rows) are high to enable the pull-up
	PORTB = KEYMX_ROWS;
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
		showNumber(theValue, 0, 2, DIGIT_SHOW_BASE);
	}
}

