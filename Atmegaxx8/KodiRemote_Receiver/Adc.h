/*
 * Adc.h
 *
 * Created: 2016/02/23 21:36:35
 *  Author: Alan
 */ 


#ifndef ADC_H_
#define ADC_H_


//Read ADC value into a 16 bits uint16
uint16_t ReadADCx16();

//Read ADC value into a 8 bits
uint8_t ReadADCx8();

void InitADC(uint8_t pAdcPin);

#endif /* ADC_H_ */