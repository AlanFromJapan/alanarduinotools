/*
 * FiniteStateMachine.c
 *
 * Created: 2016/03/18 22:20:43
 *  Author: Alan
 */ 

#include "FiniteStateMachine.h"

uint8_t getMessageCRC (uint8_t* pMsg, uint8_t pMsgLen) {
	uint8_t vCrc = pMsg[0];

	for (uint8_t i = 1; i < pMsgLen; i++) {
		vCrc += pMsg[i];
	}
	
	return vCrc;
}

uint8_t fsmIsCompleted (fsm* pFSM){
	if (((*pFSM).position * 8) >= ((*pFSM).length -1))
		return (uint8_t)1;
	else
		return (uint8_t)0;
}

void fsmInit (fsm* pFSM, uint8_t* pPattern, uint8_t pLength) {
	(*pFSM).pattern = pPattern;
	(*pFSM).position = -1;
	(*pFSM).length = pLength * 8;
	(*pFSM).crc = getMessageCRC(pPattern, pLength);
	if ((0x80 & (*pFSM).pattern[0]) == 0){
		(*pFSM).startBit = 0;		
	}
	else {
		(*pFSM).startBit = 1;
	}		
}


uint8_t fsmTransit (fsm* pFSM, uint8_t pSymbol){
	uint8_t vNextByte;
	uint8_t vNextSymbol;
	int8_t  vNextPos;
	
	//Completed FSM stay completed (open to debate, but in my case is ok)
	if (fsmIsCompleted(pFSM) == 1)
		return 1;
	
	vNextPos = (*pFSM).position +1;
	vNextByte = (*pFSM).pattern[vNextPos / 8];
		
	vNextSymbol = vNextByte & (1 << (vNextPos % 8));
	
	if ((pSymbol== 0 && vNextSymbol == 0) || (pSymbol != 0 && vNextSymbol != 0)){
		//match
		(*pFSM).position = vNextPos;
		return 1;
	}
	else{
		//back to start
		fsmReset(pFSM);
		
		//should optimize to maybe go first position in case... let's leave as is for now
		return 0;
	}
	
}

void fsmReset (fsm* pFSM){
	(*pFSM).position = -1;	
}
