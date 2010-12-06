#include "ARCPO_Lib.h"
#include "ARTools.h"

#define LED1_PIN 13
#define LED2_PIN 12
#define BUTTON_PIN 8

#define PACKET_TYPE_LED 199

#define PACKET_SUBTYPE_LED1 1
#define PACKET_SUBTYPE_LED2 2


#include "WProgram.h"
void setup ();
void loop ();
void ProcessPacketLed (ARCPOPacket& pPacket) {
  switch (pPacket.SubType){
    case PACKET_SUBTYPE_LED1:
      pika(LED1_PIN, 100, 3);
      break;
    case PACKET_SUBTYPE_LED2:
      pika(LED2_PIN, 100, 5);
      break;
    default:
      //nothing
      break;
  }
}

//Analyzes a packet and act accordingly
void ProcessPacket (ARCPOPacket& pPacket) {
  switch (pPacket.Type){
    case PACKET_TYPE_LED:
      ProcessPacketLed (pPacket);
      break;
    default:
      //nothing
      break;
  }
}


void setup (){
  Serial.begin(9600);
  
  pika(LED1_PIN, 50, 3);
}

void loop (){
  byte vButton1;
  ARCPOPacket vPacket;
  
  vButton1 = readButton(BUTTON_PIN);
  if (vButton1 == 0){
    ARCPOPacket vP;
    vP.Type = 98;
    vP.Content [0] = 'x';
    vP.Content [1] = 0;
    
    
    writePacket (vP);
    pika(LED1_PIN, 100);
  }
  
  
  vPacket = readPacket();
  
  if (readPacketSuccess()){
    delay(250);
    ProcessPacket (vPacket);
    //Serial.print ("Read a packet -->");
    //DEBUG_TRACE_ARCPOPACKET_TO_SERIAL (vPacket);
  }
  
  //system needs a small delay other keep on reading the same entries (COM problem?)
  delay(250);
}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

