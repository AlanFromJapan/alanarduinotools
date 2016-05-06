/*
 * FiniteStateMachine.h
 *
 * Created: 2016/03/18 22:20:59
 *  Author: Alan
 */ 


#ifndef FINITESTATEMACHINE_H_
#define FINITESTATEMACHINE_H_

#include <avr/io.h>

/* Forward declare a type "fsm" to be a struct. */
typedef struct fsm fsm;
/* Declare the struct  */
struct fsm {
	uint8_t startBit;
	int8_t position;
	uint8_t length;
	uint8_t crc;
	uint8_t* pattern;
};


/************************************************************************/
/* Inits the FSM                                                        */
/************************************************************************/
void fsmInit (fsm* pFSM, uint8_t* pPattern, uint8_t pLength) ;

/************************************************************************/
/* Returns 1 if finished, 0 otherwise                                   */
/************************************************************************/
uint8_t fsmTransit (fsm* pFSM, uint8_t pSymbol);

void fsmReset (fsm* pFSM);

/************************************************************************/
/* Returns 1 if finished, 0 otherwise                                   */
/************************************************************************/
uint8_t fsmIsCompleted (fsm* pFSM);

#endif /* FINITESTATEMACHINE_H_ */