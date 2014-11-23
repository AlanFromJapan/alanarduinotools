/*
 * SPI.h
 *
 * A rewriting of the Arduino's SPI library.
 * Left out the interrupt part, not needed so far.
 * 
 * Created: 2014/11/23 11:38:46
 *  Author: Alan
 */ 


#ifndef SPI_H_
#define SPI_H_


#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06
//#define SPI_CLOCK_DIV64 0x07

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

#define SPI_MODE_MASK 0x0C  // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK 0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR

#define _BV(bit) (1 << (bit))

//Additional defines 'cause I'm too lazy to search how they are called really
#define SPI_PIN_SS 2   //SS in PIN B2
#define SPI_PIN_SCK 5   //SS in PIN B5
#define SPI_PIN_MOSI 3   //SS in PIN B3
#define SPI_PIN_MISO 4   //SS in PIN B4

uint8_t spiTransfer(uint8_t _data) {
	SPDR = _data;
	while (!(SPSR & _BV(SPIF)))
	;
	return SPDR;
}


void spiBegin() {

	// Set SS to high so a connected chip will be "deselected" by default
	//digitalWrite(SS, HIGH);
	PORTB |= (1 << SPI_PIN_SS);

	// When the SS pin is set as OUTPUT, it can be used as
	// a general purpose output port (it doesn't influence
	// SPI operations).
	//pinMode(SS, OUTPUT);
	DDRB |= (1 << SPI_PIN_SS);

	// Warning: if the SS pin ever becomes a LOW INPUT then SPI
	// automatically switches to Slave, so the data direction of
	// the SS pin MUST be kept as OUTPUT.
	SPCR |= _BV(MSTR);
	SPCR |= _BV(SPE);

	// Set direction register for SCK and MOSI pin.
	// MISO pin automatically overrides to INPUT.
	// By doing this AFTER enabling SPI, we avoid accidentally
	// clocking in a single bit since the lines go directly
	// from "input" to SPI control.
	// http://code.google.com/p/arduino/issues/detail?id=888
	//pinMode(SCK, OUTPUT);
	//pinMode(MOSI, OUTPUT);
	DDRB |= (1 << SPI_PIN_SCK) | (1 << SPI_PIN_MOSI);
}

void spiEnd() {
	SPCR &= ~_BV(SPE);
}

void spiSetBitOrder(uint8_t bitOrder)
{
	if(bitOrder == LSBFIRST) {
		SPCR |= _BV(DORD);
	} else {
		SPCR &= ~(_BV(DORD));
	}
}


void spiSetDataMode(uint8_t mode)
{
	SPCR = (SPCR & ~SPI_MODE_MASK) | mode;
}

void spiSetClockDivider(uint8_t rate)
{
	SPCR = (SPCR & ~SPI_CLOCK_MASK) | (rate & SPI_CLOCK_MASK);
	SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((rate >> 2) & SPI_2XCLOCK_MASK);
}


#endif /* SPI_H_ */