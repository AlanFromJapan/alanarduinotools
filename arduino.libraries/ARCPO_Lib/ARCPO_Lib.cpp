#include "ARCPO_Lib.h"

#include "HardwareSerial.h"



bool mPacketRead = false;

bool readPacketSuccess() {
	return mPacketRead;
}

// Returns a packet read from the serial connection
void writePacket(ARCPOPacket& pPacket){
	delay(30);
	Serial.print ("$*"); 
	Serial.print (pPacket.ID);
	Serial.print (pPacket.Type);
	Serial.print (pPacket.SubType);
	Serial.print (pPacket.ExpectAcknowledge);  
	Serial.print (pPacket.Content);
	Serial.print ("*$");
	delay(30);
}


// Returns a packet read from the serial connection
ARCPOPacket& readPacketInternal(ARCPOPacket& pPacket){
	byte vPos = 0;  

#ifdef ARCPO_Lib_DEBUGMODE
	Serial.print ("DBG: avail =");
	Serial.println ((int)Serial.available());
#endif


	pPacket.ID = (char)Serial.read();
	pPacket.Type = (char)Serial.read();
	pPacket.SubType = (char)Serial.read();
	pPacket.ExpectAcknowledge = (char)Serial.read();
	while(Serial.available() && vPos < CONTENT_LENGTH) {
		char c = Serial.read();
		pPacket.Content[vPos++] = c;
		if (c == 0){ Serial.flush(); break;}
	}

	return pPacket;
}

// Returns a packet read from the serial connection
ARCPOPacket& readPacket(ARCPOPacket& pPacket){
	char vReadChar;
	boolean vStarterRead = false;
	mPacketRead = false;

	if (Serial.available() > 0) {
		//wait to make sure everything will be there
		//at 9600 bps, one need (1/9600) * 8 bits * 64 byte = 0.053 sec. We wait 0.1 sec to be sure.
		delay(100);

		while(Serial.available() > 0){
			vReadChar = Serial.read();
			
			//search the start
			if (vReadChar == '$'){
				vStarterRead = true;
			}
			else {
				//confirm the start
				if (vStarterRead && vReadChar == '*'){
					vStarterRead = false;
					pPacket = readPacketInternal(pPacket);
					mPacketRead = true;
				}
				else {
					//crap in  the buffer, clean until next starter or empty
					vStarterRead = false;
				}
			}
		}
	}
#ifdef ARCPO_Lib_DEBUGMODE
	else {
		Serial.print ("DBG: readPacket : no data => ");
		Serial.println ((int)Serial.available());
	}
#endif    

	if (mPacketRead && pPacket.ExpectAcknowledge == 1){
#ifdef ARCPO_Lib_DEBUGMODE
		Serial.print ("Acknowledgement -->");
#endif
		//acknowledge packet
		ARCPOPacket vPacketAck;
		vPacketAck.ID = pPacket.ID;
		vPacketAck.Type = 255; 
		vPacketAck.SubType = 255; 
		vPacketAck.ExpectAcknowledge = 255;  
		vPacketAck.Content[0] = 'A';
		vPacketAck.Content[1] = 'C';
		vPacketAck.Content[2] = 'K';
		vPacketAck.Content[3] = 0;
		writePacket(vPacketAck);
	}

	return pPacket;
}

//DEBUG function 
void DEBUG_TRACE_ARCPOPACKET_TO_SERIAL (ARCPOPacket& pPacket){
	Serial.print ("ID: "); Serial.print ((int)pPacket.ID);
	Serial.print ("Type: "); Serial.print ((int)pPacket.Type);
	Serial.print ("SubType: "); Serial.print ((int)pPacket.SubType);
	Serial.print ("Ack: "); Serial.println ((int)pPacket.ExpectAcknowledge);

	Serial.println(pPacket.Content);
	//writePacket (pPacket);
}

