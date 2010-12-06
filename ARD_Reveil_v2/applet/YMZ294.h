#include "WProgram.h"
#include "wiring.h"

/*
* Uses the Yamaha YMZ294 with a shift register CD4015 SIPO (Serial in, parallel out)
* Meaning the data 8 bits of the YMZ294 are written with just 2 pins that drive the CD4015!
* = 6 pins saved ... it worth a soldering.
* Doc: http://kalshagar.wikispaces.com/Arduino+and+a+YMZ294
* and http://d.hatena.ne.jp/kazuya_a/20090625/1245926938
*/

#ifndef __YMZ294_H__
#define __YMZ294_H__

//YMZ294
//D0-D7 = 0-7pin
#define YMZ294_WR 13
#define YMZ294_CS 12
#define YMZ294_A0 11

#define ADDR_FREQ_A 0x00
#define ADDR_FREQ_B 0x02
#define ADDR_FREQ_C 0x04
#define ADDR_NOISE 0x06
#define ADDR_MIXER 0x07
#define ADDR_LEVEL_A 0x08
#define ADDR_LEVEL_B 0x09
#define ADDR_LEVEL_C 0x0A
#define ADDR_FREQ_E 0x0B
#define ADDR_SHAPE_E 0x0D



int tp[] = {//MIDI note number
  15289, 14431, 13621, 12856, 12135, 11454, 10811, 10204,//0-7
  9631, 9091, 8581, 8099, 7645, 7215, 6810, 6428,//8-15
  6067, 5727, 5405, 5102, 4816, 4545, 4290, 4050,//16-23
  3822, 3608, 3405, 3214, 3034, 2863, 2703, 2551,//24-31
  2408, 2273, 2145, 2025, 1911, 1804, 1703, 1607,//32-39
  1517, 1432, 1351, 1276, 1204, 1136, 1073, 1012,//40-47
  956, 902, 851, 804, 758, 716, 676, 638,//48-55
  602, 568, 536, 506, 478, 451, 426, 402,//56-63
  379, 358, 338, 319, 301, 284, 268, 253,//64-71
  239, 225, 213, 201, 190, 179, 169, 159,//72-79
  150, 142, 134, 127, 119, 113, 106, 100,//80-87
  95, 89, 84, 80, 75, 71, 67, 63,//88-95
  60, 56, 53, 50, 47, 45, 42, 40,//96-103
  38, 36, 34, 32, 30, 28, 27, 25,//104-111
  24, 22, 21, 20, 19, 18, 17, 16,//112-119
  15, 14, 13, 13, 12, 11, 11, 10,//120-127
  0//off
};


//CD4015 dual 4bit shift register SIPO
//mounted as a single 8bit shift register SIFO
#define CD4015_CLOCK 2
#define CD4015_DATA 3
#define CD4015_RESET -1 //NOT USED


//Shift register write methof
void shiftReg_Write (byte pByte){
  //i must be a int, if you put a byte if crash on the uC
  for (int i = 7 ; i >= 0; i--){
    byte v = (pByte >> i) & B00000001;

    if (v == 1){
      digitalWrite(CD4015_DATA, HIGH);
    }
    else {
      digitalWrite(CD4015_DATA, LOW);
    }

    digitalWrite(CD4015_CLOCK, LOW);
    delayMicroseconds(15); //needs average of 15us
    digitalWrite(CD4015_CLOCK, HIGH);
    delayMicroseconds(15); //needs average of 15us

#ifdef DEBUGSERIAL
    //Serial.print(v, DEC);
#endif
  }
#ifdef DEBUGSERIAL
  //Serial.print("   ");
#endif
}

//Shift register related : setup 
void setupCD4015(){
  pinMode(CD4015_DATA, OUTPUT);
  pinMode(CD4015_CLOCK, OUTPUT);
  //pinMode(CD4015_RESET, OUTPUT); //NOT USED
}

//YMZ294 related : writes data through the shift register 
void write_data(unsigned char address, unsigned char data)
{
  //write address
  digitalWrite(YMZ294_WR, LOW);
  digitalWrite(YMZ294_CS, LOW);
  digitalWrite(YMZ294_A0, LOW);

  shiftReg_Write(address);

  digitalWrite(YMZ294_WR, HIGH);
  digitalWrite(YMZ294_CS, HIGH);
  //write data
  digitalWrite(YMZ294_WR, LOW);
  digitalWrite(YMZ294_CS, LOW);
  digitalWrite(YMZ294_A0, HIGH);

  shiftReg_Write(data);

  digitalWrite(YMZ294_WR, HIGH);
  digitalWrite(YMZ294_CS, HIGH);
}

//YMZ294 related : sets the register 
void set_ch(unsigned char address, int i)
{
  write_data(address, tp[i]&0xff);
  write_data(address+1, (tp[i] >> 8)&0x0f);
}

void setVolumeYMZ294(byte pVolume){
  write_data(ADDR_LEVEL_A, pVolume & 0x0f);
  write_data(ADDR_LEVEL_B, pVolume & 0x0f);
  write_data(ADDR_LEVEL_C, pVolume & 0x0f);  
}

//makes the setup for the Yamaha YMZ294 component
void setupYMZ294(){
  pinMode(YMZ294_WR, OUTPUT);
  pinMode(YMZ294_CS, OUTPUT);
  pinMode(YMZ294_A0, OUTPUT);

  //no noise 
  write_data(ADDR_NOISE, 0x00);
  //mixer
  //write_data(ADDR_MIXER, 0x3e);
  write_data(ADDR_MIXER, 0x38);

  //set levels
  setVolumeYMZ294(0x00);  
}


void testSR(){

  for (byte i = 0; i < 32 ; i++){
    shiftReg_Write(i);
    Serial.print(i, DEC);
    Serial.print(",");
    delay (1000);
  }
}


#endif __YMZ294_H__
