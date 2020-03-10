/*
 * flash_utils.h
 *
 *  Created on: Mar 9, 2020
 *      Author: alan
 */

#ifndef FLASH_UTILS_H_
#define FLASH_UTILS_H_

#define PORT_ADDR_H			PORTE
#define PORT_ADDR_M			PORTC
#define PORT_ADDR_L			PORTF
#define PORT_DATA			PORTA
#define PORT_CONTROL		PORTB
#define PIN_CE				0x08
#define PIN_OE				0x10
#define PIN_WE				0x20
#define PIN_RST				0x40
//mask to enable result of only our control pins
//#define PIN_CONTROLS_MASK	(PIN_RST)

void setupExternalFlash();

uint8_t flashGetByteDecode() ;
void flashWriteByteDecode() ;
void flashWriteByteDecode2(uint16_t pAddr, uint8_t pData) ;
uint8_t flashGetByteDecode2(uint16_t pAddr) ;

void flashReset();
void flashErase();

#endif /* FLASH_UTILS_H_ */
