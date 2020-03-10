/*
 * flash_utils.c
 *
 *  Created on: Mar 9, 2020
 *      Author: alan
 */


#define TALKATIVE

#include <avr/io.h>
#include <avr/iom64.h>
#include <util/delay.h>
//#include <avr/interrupt.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "serial_utils.h"
#include "flash_utils.h"



void setDataBusWrite() {
	PORT_CONTROL |= PIN_RST;

	DDRA = 0xFF;
	PORTA = 0x00;
	//PINA = 0x00;
}
void setDataBusRead() {
	PORT_CONTROL |= PIN_RST;

	DDRA = 0x00;
	//force pullups
	SFIOR &= ~(1 << PUD);
	PORTA = 0xFF ;
	//PINA = 0x00;
}

void setupExternalFlash(){
	//address 0..7
	DDRF = 0xFF;
	//address 8..15
	DDRC = 0xFF;
	//address 16..17
	DDRE = 0xC0; //sets PE7 and PE6

	//commands PB3..PB6
	DDRB = 0x78;

	//data bus bi-directional so will change
	setDataBusWrite();

	//RESET flash
	PORT_CONTROL &= ~PIN_RST;
	_delay_ms(10);
	PORT_CONTROL |= PIN_RST;
	_delay_ms(10);

	PORT_CONTROL = PIN_RST | PIN_WE | PIN_OE;
}



void setAddressOnBus( uint16_t pAddr ) {
	//put the address
	PORT_ADDR_L = (uint8_t)pAddr;
	PORT_ADDR_M = (uint8_t)(pAddr >> 8);
	PORT_ADDR_H = 0x00; //PORT_ADDR_H & 0x3f;
}

uint16_t getAddressFromSerial() {
	uint8_t inByte = 0;         // incoming serial byte
	uint16_t vAddress = 0x0000;

	inByte = hexToInt(serialRead());
	inByte = inByte << 4;
	inByte = inByte | hexToInt(serialRead());

	vAddress = inByte;
	vAddress = vAddress << 8;

	inByte = hexToInt(serialRead());
	inByte = inByte << 4;
	inByte = inByte | hexToInt(serialRead());

	vAddress = vAddress | (uint16_t)inByte;

	return vAddress;
}

uint8_t flashGetDataFromInput() {
	uint8_t inByte = 0;         // incoming serial byte

	inByte = hexToInt(serialRead());
	inByte = inByte << 4;
	inByte = inByte | hexToInt(serialRead());
	//don't set it now, we have to respect a proper sequence for writing
	//PORTA = inByte;

	return inByte;
}

#ifdef TALKATIVE
void debugPrint(uint16_t pAddr, uint8_t pData){
	char vBuffChar[6];

	serialWriteString("addr: 0x");

	sprintf(vBuffChar, "%04x", pAddr);
	serialWriteString(vBuffChar);

	serialWriteString("  Value: 0x");

	sprintf(vBuffChar, "%02x", pData);
	serialWriteString(vBuffChar);

}
#endif //TALKATIVE

/************************************************************************/
/* GET byte                                                             */
/************************************************************************/
uint8_t flashGetByteDecode2(uint16_t pAddr) {
	//read FLASH
	setDataBusRead();

	//Doc: READ = /CE(l) & /WE(h) & /OE(l) & /RST(h)
	PORT_CONTROL = (PIN_RST  | PIN_OE | PIN_WE );
	_delay_us(1);

	////WE goes high
	//PORT_CONTROL = PORT_CONTROL | PIN_WE;
	//_delay_us(1);
//
	//put the address
	setAddressOnBus(pAddr);
	_delay_us(1);


	//OE goes low
	PORT_CONTROL &= ~PIN_OE;
	//doc says tOE = 0ns
	_delay_us(0.2);


	//read !!
	uint8_t vData = PINA;
	_delay_us(0.2);

	//OE & CE goes high
	PORT_CONTROL |= PIN_OE;
	_delay_us(1);

	////WE goes low
	//PORT_CONTROL = PORT_CONTROL & ~PIN_WE;
	//_delay_us(1);


#ifdef TALKATIVE

	serialWriteString("GET ");
	debugPrint(pAddr, vData);
	serialWriteString("\n");

#endif //TALKATIVE
	return vData;
}
uint8_t flashGetByteDecode() {
	uint16_t vAddress = getAddressFromSerial();

	return flashGetByteDecode2(vAddress);
}



/************************************************************************/
/* WRITE byte                                                           */
/* Input: -                                                             */
/* Output: CONTROL = OE high                                            */
/************************************************************************/
void flashWriteSeq1Byte (uint16_t pAddr, uint8_t pData){
	//Doc: READ = /CE(l) & /WE(l) & /OE(h) & /RST(h)
	//but watch the sequence



	//Start : CE and WE high
	PORT_CONTROL = (PIN_RST  | PIN_WE | PIN_OE | PIN_CE);
	_delay_us(1);

	setAddressOnBus(pAddr);
	_delay_us(1);

	//CE goes low
	//PORT_CONTROL = PORT_CONTROL & ~PIN_CE;
	//_delay_us(1);
//
	////OE goes high
	//PORT_CONTROL = PORT_CONTROL | PIN_OE;
	//_delay_us(1);
	//
	//WE goes low
	PORT_CONTROL &= ~PIN_WE;
	_delay_us(1);
	//CE goes low
	PORT_CONTROL &= ~PIN_CE;
	_delay_us(1);

	//put the data
	PORT_DATA = pData;
	_delay_us(1);

	//WE goes high
	PORT_CONTROL |= PIN_WE;
	_delay_us(50);
	//CE goes high
	PORT_CONTROL |= PIN_CE;
	_delay_us(50);


}

void waitForDataConfirmation( uint8_t pData )
{
		//wait for data ??
	setDataBusRead();
	uint8_t vCheckVal = 0;
	while(1){
		//OE low
		PORT_CONTROL = PORT_CONTROL & ~PIN_OE;
		_delay_us(1);
		//read data bus
		vCheckVal = PINA;

		//OE up
		PORT_CONTROL = PORT_CONTROL | PIN_OE;
		_delay_us(1);

		if (pData == vCheckVal)
			break;
	}
}


void flashWriteByteDecode2(uint16_t pAddr, uint8_t pData) {

	//go write mode
	setDataBusWrite();

	flashWriteSeq1Byte(0x5555, 0xaa);
	flashWriteSeq1Byte(0x2aaa, 0x55);
	flashWriteSeq1Byte(0x5555, 0xa0);
	flashWriteSeq1Byte(pAddr, pData);

	//waitForDataConfirmation(pData);

	//back to read mode
	setDataBusRead();


#ifdef TALKATIVE
	serialWriteString("SET ");

	debugPrint(pAddr, pData);

	serialWriteString("\n");


	//
	//serialWriteString("Confirm addr: 0x");
	//serialWriteString(itoa(PORT_ADDR_M, vBuffChar, 16));
	//serialWriteString(itoa(PORT_ADDR_L, vBuffChar, 16));
	//serialWriteString("\n");
#endif //TALKATIVE
}


void flashWriteByteDecode() {
	//address
	uint16_t vAddress = getAddressFromSerial();
	uint8_t vData = flashGetDataFromInput();

	flashWriteByteDecode2(vAddress, vData);
}
