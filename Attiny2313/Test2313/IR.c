//#include "IR/IR_NEC.h"
#include "IR/IR_Sharp.h"
#include "IR/IR_NEC.h"

void TestVCROnOff() {
	//VCR is a Buffalo something and using a NEC extended format
	//POWER 30EEB14E
	//2 first bytes are the unique ID, the two last bytes are the command and it's binary opposite
	
	//transmit_nec_ir_commandExt(0x30EE, 0xB1);
	
	

}	

void TestTVOnOff(){
		    //TV power, works ok
		    //IRSendSharp (0x41A2, 15);
			
			//vol- ok
			//IRSendSharp (0x42A2, 15);
			
			//vol+ ok
			//IRSendSharp (0x40A2, 15);
			
			//input change ok
			//IRSendSharp (0x4322, 15);
	
}