
#ifndef __DS3234_h__
#define __DS3234_h__

//Include also in the MAIN .ino file!
#include "SPI.h"
#include "MCMShared.h"

#include <util/delay.h>



#define RTC_DS3234

#ifdef RTC_DS3234
	#define RTC_READ_TIME(p)	ReadTime3234(p)
	#define RTC_INIT()			setupDS3234(1)
	#define RTC_SET_TIME(p)		SetTimeDate3234_2(p)
#endif


//Chip select pin. You must use this, front detection is necessary
//#define DS3234_PIN_CS 10  --> SPI_PIN_SS = PIN B2


//=====================================
uint8_t setupDS3234(uint8_t pSetRegisters){ 
   //pinMode(DS3234_PIN_CS,OUTPUT); // chip select
   DDRB |= (1 << SPI_PIN_SS);
   
   // start the SPI library:
   spiBegin();
   spiSetBitOrder(MSBFIRST); 
   spiSetDataMode(SPI_MODE1); // both mode 1 & 3 should work 
   if (pSetRegisters != 0){
	   //set control register 
	   PORTB &= ~(1 << SPI_PIN_SS); //digitalWrite(DS3234_PIN_CS, LOW);  
	   spiTransfer(0x8E);
	   spiTransfer(0x60); //60= disable Osciallator and Battery SQ wave @1hz, temp compensation, Alarms disabled
	   PORTB |= (1 << SPI_PIN_SS); //digitalWrite(DS3234_PIN_CS, HIGH);
   }
   
   _delay_ms(10); //delay(10);

   return 0;
}



uint8_t SetTimeDate3234_2(Date* pDateTime){
	SetTimeDate3234(
		(*pDateTime).dayOfMonth,
		(*pDateTime).month,
		(*pDateTime).year,
		(*pDateTime).hour,
		(*pDateTime).minute,
		(*pDateTime).second
	);
}
	
//=====================================
//Taken from Sparkfun sample (as is)
uint8_t SetTimeDate3234(uint8_t d, uint8_t mo, uint16_t y, uint16_t h, uint16_t mi, uint16_t s){ 
   uint16_t TimeDate [7]={
      s,mi,h,0,d,mo,y               };
   for(uint8_t i=0; i<=6;i++){
      if(i==3)
         i++;
      uint16_t b= TimeDate[i]/10;
      uint16_t a= TimeDate[i]-b*10;
      if(i==2){
         if (b==2)
            b=0b00000010;
         else if (b==1)
            b=0b00000001;
      }	
      TimeDate[i]= a+(b<<4);

      PORTB &= ~(1 << SPI_PIN_SS); //digitalWrite(DS3234_PIN_CS, LOW);
      spiTransfer(i+0x80); 
      spiTransfer(TimeDate[i]);        
      PORTB |= (1 << SPI_PIN_SS); //digitalWrite(DS3234_PIN_CS, HIGH);
   }

   return 0;
}




//=====================================
//Taken from Sparfun sample (amended to return values and not a string)
//Parameter you pass must be a correctly initialized 7 or more uint8_t array
void ReadTime3234(Date* pTimeDate){
   //uint8_t TimeDate [7]; //second,minute,hour,null,day,month,year		
   for(uint8_t i=0; i<=6;i++){
      if(i==3)
         i++;
      PORTB &= ~(1 << SPI_PIN_SS); //digitalWrite(DS3234_PIN_CS, LOW);
      spiTransfer(i+0x00); 
      uint16_t n = spiTransfer(0x00);        
      PORTB |= (1 << SPI_PIN_SS); //digitalWrite(DS3234_PIN_CS, HIGH);
      uint16_t a=n & 0b00001111;    
      if(i==2){	
         uint16_t b=(n & 0b00110000)>>4; //24 hour mode
         if(b==0b00000010)
            b=20;        
         else if(b==0b00000001)
            b=10;
         (*pTimeDate).hour=a+b;
      }
      else if(i==4){
         uint16_t b=(n & 0b00110000)>>4;
         (*pTimeDate).dayOfMonth=a+b*10;
      }
      else if(i==5){
         uint16_t b=(n & 0b00010000)>>4;
         (*pTimeDate).month=a+b*10;
      }
      else if(i==6){
         uint16_t b=(n & 0b11110000)>>4;
         (*pTimeDate).year=a+b*10;
      }
      else{ 
         if (i == 0) {
            uint16_t b=(n & 0b01110000)>>4;
            (*pTimeDate).second = a+b*10;	
         }	
         if (i == 1) {
            uint16_t b=(n & 0b01110000)>>4;
            (*pTimeDate).minute = a+b*10;	
         }	

      }
   }
}

#endif // __DS3234_h__


