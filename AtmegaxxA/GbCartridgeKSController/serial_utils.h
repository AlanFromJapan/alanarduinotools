/*
 * serial_utils.h
 *
 *  Created on: Mar 9, 2020
 *      Author: alan
 */

#ifndef SERIAL_UTILS_H_
#define SERIAL_UTILS_H_

#define BaudRate 57600
#define MYUBRR (F_CPU / 16 / BaudRate ) - 1


#define ASCII0  48
#define ASCII9  57
#define ASCIIA  65
#define ASCIIa	97


void setupSerial();
void establishContact();
unsigned char serialCheckRxComplete(void);
unsigned char serialRead(void);
void serialWrite(unsigned char DataOut);
void serialWriteString(const char* pString);

uint8_t hexToInt(uint8_t pChar);

#endif /* SERIAL_UTILS_H_ */
