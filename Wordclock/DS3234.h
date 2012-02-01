#include "Arduino.h"

#ifndef __DS3234_h__
#define __DS3234_h__

//Include also in the MAIN .ino file!
#include "SPI.h"
//Chip select pin. You must use this, front detection is necessary
#define DS3234_PIN_CS 10



//=====================================
int RTC_init(boolean pSetRegisters){ 
   pinMode(DS3234_PIN_CS,OUTPUT); // chip select
   // start the SPI library:
   SPI.begin();
   SPI.setBitOrder(MSBFIRST); 
   SPI.setDataMode(SPI_MODE1); // both mode 1 & 3 should work 
   if (pSetRegisters){
	   //set control register 
	   digitalWrite(DS3234_PIN_CS, LOW);  
	   SPI.transfer(0x8E);
	   SPI.transfer(0x60); //60= disable Osciallator and Battery SQ wave @1hz, temp compensation, Alarms disabled
	   digitalWrite(DS3234_PIN_CS, HIGH);
   }
   delay(10);

   return 0;
}
//=====================================
//Taken from Sparfun sample (as is)
int SetTimeDate(int d, int mo, int y, int h, int mi, int s){ 
   int TimeDate [7]={
      s,mi,h,0,d,mo,y               };
   for(int i=0; i<=6;i++){
      if(i==3)
         i++;
      int b= TimeDate[i]/10;
      int a= TimeDate[i]-b*10;
      if(i==2){
         if (b==2)
            b=B00000010;
         else if (b==1)
            b=B00000001;
      }	
      TimeDate[i]= a+(b<<4);

      digitalWrite(DS3234_PIN_CS, LOW);
      SPI.transfer(i+0x80); 
      SPI.transfer(TimeDate[i]);        
      digitalWrite(DS3234_PIN_CS, HIGH);
   }

   return 0;
}



//=====================================
//Taken from Sparfun sample (amended to return values and not a string)
//Parameter you pass must be a correctly initialized 7 or more int array
void ReadTimeArray(int* TimeDate){
   //int TimeDate [7]; //second,minute,hour,null,day,month,year		
   for(int i=0; i<=6;i++){
      if(i==3)
         i++;
      digitalWrite(DS3234_PIN_CS, LOW);
      SPI.transfer(i+0x00); 
      unsigned int n = SPI.transfer(0x00);        
      digitalWrite(DS3234_PIN_CS, HIGH);
      int a=n & B00001111;    
      if(i==2){	
         int b=(n & B00110000)>>4; //24 hour mode
         if(b==B00000010)
            b=20;        
         else if(b==B00000001)
            b=10;
         *(TimeDate +i)=a+b;
      }
      else if(i==4){
         int b=(n & B00110000)>>4;
         *(TimeDate +i)=a+b*10;
      }
      else if(i==5){
         int b=(n & B00010000)>>4;
         *(TimeDate +i)=a+b*10;
      }
      else if(i==6){
         int b=(n & B11110000)>>4;
         *(TimeDate +i)=a+b*10;
      }
      else{	
         int b=(n & B01110000)>>4;
         *(TimeDate +i)=a+b*10;	
      }
   }
}


#endif // __DS3234_h__


