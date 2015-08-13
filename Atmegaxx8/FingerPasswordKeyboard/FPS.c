/*
 * FPS.c
 *
 * Created: 2015/07/13 22:08:51
 *  Author: Alan
 */ 

#include "FPS.h"
#include "serialComm.h"

#include <avr/io.h>

#define FPS_INIT			0x01
#define FPS_CLOSE			0x02
#define FPS_LIGHT_TOGGLE	0x12
#define FPS_ENROLL_CHECK	0x21
#define FPS_ENROLL_START	0x22
#define FPS_ENROLL_1		0x23
#define FPS_ENROLL_2		0x24
#define FPS_ENROLL_3		0x25
#define FPS_FINGER_PRESSED	0x26
#define FPS_VERIFY			0x50
#define FPS_CAPTURE_FINGER 	0x60

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

#define FPS_RESPONSE_LEN	12
#define FPS_RESPONSE_ACK	0x30
#define FPS_RESPONSE_NACK	0x31

uint32_t mFPSLatestResponseValue = 0x00000000;
uint8_t mFPSLatestResponseStatus = FPS_RESPONSE_NACK;

//reads last message and store in buffer
//Returns 0 if error, 1 if ok
static uint8_t fpsReceive(){
	uint8_t mLatestResponse[FPS_RESPONSE_LEN];
	mFPSLatestResponseStatus = FPS_RESPONSE_NACK;
	
	//read
	for (uint8_t i =0 ; i < FPS_RESPONSE_LEN; i++){
		mLatestResponse[i] = USART_Receive();
	}
	
	//now check
	//[0] = 0x55 & [1] = 0xAA
	if (mLatestResponse[0] != 0x55 || mLatestResponse[1] != 0xAA){
		return 0;
	}
	//too lazy to do the checksum
	
	//store values
	//Response code should be ACK ok NACK
	mFPSLatestResponseStatus = mLatestResponse[8]; // it's LSB but values are <255 so reading this byte only is ok
	//The response parameter, will be error code or real value
	mFPSLatestResponseValue = mLatestResponse[7];
	mFPSLatestResponseValue = mFPSLatestResponseValue << 8;
	mFPSLatestResponseValue = mFPSLatestResponseValue + mLatestResponse[6];
	mFPSLatestResponseValue = mFPSLatestResponseValue << 8;
	mFPSLatestResponseValue = mFPSLatestResponseValue + mLatestResponse[5];
	mFPSLatestResponseValue = mFPSLatestResponseValue << 8;
	mFPSLatestResponseValue = mFPSLatestResponseValue + mLatestResponse[4];

	//if not ok assume failed
	if (mFPSLatestResponseStatus == FPS_RESPONSE_ACK){
		return 1;
	}
	else {
		return 0;
	}
}

//open communication to FPS
void fpsInit(){
	//send !=0 to get "extra info"
	fpsSend(0x0000, FPS_INIT);
}

//closes communication to FPS
//Doc says it does nothing, so is it worth using?
void fpsClose(){
	fpsSend(0x0000, FPS_CLOSE);
}

//turns light on/off
void fpsSetLight (uint8_t pValue) {
	USART_Flush();
	fpsSend((uint16_t)pValue, FPS_LIGHT_TOGGLE);
	fpsReceive();
}

//Check enrollment of ID [0;19]
//ACK means enrolled, NACK -> check if response is not used
void fpsEnrollCheck(uint8_t pID){
	USART_Flush();
	fpsSend((uint16_t)pID, FPS_ENROLL_CHECK);
	fpsReceive();
}

//start enrollment at ID [0;19]
//can't override, do delete before re-writing
void fpsEnrollStart(uint8_t pID){
	fpsSend((uint16_t)pID, FPS_ENROLL_START);
}

//Returns 0 if issue, something else otherwise
//pStep is between 1 and 3
uint8_t fpsEnroll(uint8_t pStep){
	fpsSend(0x0000, FPS_ENROLL_1 - 1 + pStep);
}

//Returns 0 if finger pressed, 1 otherwise
uint8_t fpsIsFingerPressed(){
	fpsSend(0x0000, FPS_FINGER_PRESSED);
	return 0;
}

//Capture an image of the finger and keep in memory
//Use BestQuality = 1 for enrollment and 0 for identification
void fpsCaptureFinger(uint8_t pBestQuality){
	fpsSend((uint16_t)pBestQuality, FPS_CAPTURE_FINGER);
}