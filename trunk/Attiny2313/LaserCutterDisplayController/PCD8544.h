/*
 * PCD8544.h
 *
 * Created: 2013/10/26 18:13:01
 *  Author: Alan
 */ 


#ifndef PCD8544_H_
#define PCD8544_H_


#define LCD_CLK		6
#define LCD_DIN		5
#define LCD_DC		4
#define LCD_RESET	3
#define LCD_SCE		2

#define LCD_WIDTH	84
#define LCD_HEIGHT	48

#define PCD8544_CMD  0
#define PCD8544_DATA 1

#define LCD_PORT	PORTD
#define LCD_DIR		DDRD

uint8_t mLcdColumn;
uint8_t mLcdLine;


void LcdSetup();
void LcdSend (uint8_t pType, uint8_t pData);
void LcdClear();
void LcdSetCursor(uint8_t column, uint8_t line);
void LcdSetPower(uint8_t on);
void LcdSetInverse(uint8_t inverse);

#endif /* PCD8544_H_ */