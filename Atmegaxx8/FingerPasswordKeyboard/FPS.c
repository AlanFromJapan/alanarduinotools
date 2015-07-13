/*
 * FPS.c
 *
 * Created: 2015/07/13 22:08:51
 *  Author: Alan
 */ 

#include "FPS.h"
#include "serialComm.h"

#include <avr/io.h>

static void fpsSend(uint16_t pParam, uint8_t pCommand){
	//https://github.com/mlaws/GT-511C1_Mega/blob/master/fingerprint.ino
	uint16_t vChecksum = 0;
	vChecksum = 0x55+0xAA+0x01+(uint8_t)pParam+((uint8_t)(pParam >> 8))+pCommand;
	
	USART_Transmit((uint8_t)0x55); //Command start code 1
	USART_Transmit((uint8_t)0xAA); //Command start code 2
	USART_Transmit((uint8_t)0x01); // This is the first byte for the device ID. It is the word 0x0001
	USART_Transmit((uint8_t)0x00); // Second byte of Device ID. Notice the larger byte is first. I'm assuming this is because the datasheet says "Multi-byte item is represented as Little Endian"
	USART_Transmit((uint8_t)pParam); //writing the largest byte of the Parameter
	USART_Transmit((uint8_t)(pParam >> 8)); //Writing the second largest byte of the Parameter
	USART_Transmit((uint8_t)0x00); //The datasheet says the parameter is a DWORD, but it never seems to go over the value of a word
	USART_Transmit((uint8_t)0x00); //so I'm just sending it a word of data. These are the 2 remaining bytes of the Dword
	USART_Transmit((uint8_t)pCommand); //write the command byte
	USART_Transmit((uint8_t)0x00); //again, the commands don't go over a byte, but it is sent as a word, so I'm only sending a byte
	USART_Transmit((uint8_t)vChecksum); //Writes the largest byte of the checksum
	USART_Transmit((uint8_t)(vChecksum >> 8)); //writes the smallest byte of the checksum
}


//open communication to FPS
void fpsInit(){
	fpsSend(0x0000,0x01);
}

//turns light on/off
void fpsSetLight (uint8_t pValue) {
	fpsSend((uint16_t)pValue,0x12);
}