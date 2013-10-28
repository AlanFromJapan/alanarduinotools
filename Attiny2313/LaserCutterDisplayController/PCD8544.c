/*
 * PCD8544.c
 *
 * Created: 2013/10/26 18:14:27
 *  Author: Alan
 */ 
/*
 * Based in library from :
 * PCD8544 - Interface with Philips PCD8544 (or compatible) LCDs.
 *
 * Copyright (c) 2010 Carlos Rodrigues <cefrodrigues@gmail.com>
 *
 */
//#include <avr/pgmspace.h>
#include <avr/delay.h>
#include <avr/io.h>
#include "PCD8544.h"



void LcdSetup(){
	
	//in my case they are all on PORTD
	//Pins go output	
	LCD_DIR |= (1 << LCD_CLK) | (1 << LCD_DC) | (1 << LCD_DIN) | (1 << LCD_RESET) | (1 << LCD_SCE);
	
	//reset port to 0
	LCD_PORT = 0x00;
	
	//reset the controller state (reset =1, sce = 1, reset= 0)
	LCD_PORT = (1 << LCD_RESET) | (1 << LCD_SCE);
	LCD_PORT &= ~(1 << LCD_RESET);
	_delay_ms(5);
	LCD_PORT |= (1 << LCD_RESET);

	//Send lcd params
	LcdSend(PCD8544_CMD, 0x21); // extended instruction set control (H=1), power on, horizontal addressing
	LcdSend(PCD8544_CMD, 0x14); // bias system (1:40/1:34)

    
	LcdSend(PCD8544_CMD, 0xC2);// default Vop (3.06 + 66 * 0.06 = 7V)	


    LcdSend(PCD8544_CMD, 0x20);  // extended instruction set control (H=0)
	
	//original code sends 0x09 which makes all black (command "all display segments on" according doc)
    LcdSend(PCD8544_CMD, 0x0C);  // normal display 


	
	// Clear RAM contents...
	//LcdClear();
	
	// Activate LCD...
	LcdSend(PCD8544_CMD, 0x08);  // display blank
	LcdSend(PCD8544_CMD, 0x0c);  // normal mode (0x0d = inverse mode)
	_delay_ms(100);
	
    // Place the cursor at the origin...
    LcdSetCursor(0, 0);	
}


void LcdSend (uint8_t pType, uint8_t pData){
	if (pType == PCD8544_DATA){
		LCD_PORT |= (1 << LCD_DC);
	}
	else {
		//PCD8544_CMD
		LCD_PORT &= ~(1 << LCD_DC);
	}
	
	//sce low
	LCD_PORT &= ~(1 << LCD_SCE);

	//send data with MSB first
	for(uint8_t i= 0; i < 8; i++){

		//tick the clock L->H
		LCD_PORT &= ~(1 << LCD_CLK);
		
		//send the MSB
		if ((pData & 0x80) == 0){
			LCD_PORT &= ~(1 << LCD_DIN);
		}
		else {
			LCD_PORT |= (1 << LCD_DIN);
		}
		
		
		//go up so READ		
		LCD_PORT |= (1 << LCD_CLK);
		//no wait
		//_delay_us(100);

		//shift left the data
		pData = pData << 1;
	}
	

	//sce high
	LCD_PORT |= (1 << LCD_SCE);	
}

void LcdClear()
{
	LcdSetCursor(0, 0);

	for (uint8_t i = 0; i < LCD_WIDTH ; i++) {
		for (uint8_t j =0; j <LCD_HEIGHT/8; j++){
			LcdSend(PCD8544_DATA, 0x00);
		}		
	}

	LcdSetCursor(0, 0);
}

void LcdSetCursor(uint8_t column, uint8_t line)
{
	mLcdColumn = (column % LCD_WIDTH);
	mLcdLine = (line % (LCD_HEIGHT/8));

	LcdSend(PCD8544_CMD, 0x80 | mLcdColumn);
	LcdSend(PCD8544_CMD, 0x40 | mLcdLine);
}

void LcdSetPower(uint8_t on)
{
	LcdSend(PCD8544_CMD, on != 0 ? 0x20 : 0x24);
}

void LcdSetInverse(uint8_t inverse)
{
	LcdSend(PCD8544_CMD, inverse != 0 ? 0x0d : 0x0c);
}