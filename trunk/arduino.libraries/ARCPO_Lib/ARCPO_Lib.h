/*
* ARCPO : ARduino Communication ProtOcol
* Attempt to create a packet-based communication framework for the Arduino 2009
* by Alain VIZZINI (may 2009)
* Free licensing - redistribute and use freely
*
* VERSION : 0.3
*
*/

#ifndef ARCPO_Lib_h
#define ARCPO_Lib_h

//#define ARCPO_Lib_DEBUGMODE

#include <WProgram.h>

#define CONTENT_LENGTH 56
//length WITHOUT the header 2 chars
#define FULL_PACKET_LENGTH CONTENT_LENGTH + 4 

//Each packet is sent with that signature $*ITEContent....
//A packet is 64 in length, internal serial buffer is 128 according documentation, which is a maximumif you dont
//want to loose too much packet I assume.
typedef struct {
  char ID; //default undefined
  char Type; 
  char SubType; 
  char ExpectAcknowledge;  
  char Content[CONTENT_LENGTH];
} ARCPOPacket;



///////////////////////////////////////////////////////////////////////////////////
// Signatures declaration
//
void writePacket(ARCPOPacket& pPacket);
bool readPacketSuccess();
ARCPOPacket& readPacket(ARCPOPacket& pPacket);
void DEBUG_TRACE_ARCPOPACKET_TO_SERIAL (ARCPOPacket& pPacket);

#endif /* ARCPO_Lib_h */