/*
 * IR_Shared.h
 *
 * Created: 2013/01/14 0:04:20
 *  Author: Alan
 */ 


#ifndef IR_SHARED_H_
#define IR_SHARED_H_

//Change this in case your IR led is not on D6.
//D0 is the /Enable of the opamp so let it OFF (means high) when sending the message
#define set_ir_led_on()  PORTD = 0x41;
#define set_ir_led_off() PORTD = 0x01;

/*TEST*/
//#define set_ir_led_on()  PORTD = 0xFF;
//#define set_ir_led_off() PORTD = 0x00;


#endif /* IR_SHARED_H_ */