#include "WProgram.h"
#include "wiring.h"

/*
*
*/

#ifndef __REVEILBUTTONS_H__
#define __REVEILBUTTONS_H__

//ALL ANALOG PINS
#define PIN_BUTTON_BIG 0
#define PIN_LIGHT 1
#define PIN_BUTTON_RED 2
#define PIN_BUTTON_BLACK 3

//prepares the button pin
void setupButtons(){
  //Shift the ANALOG pin number to DIGITAL pin (+14)
  pinMode(PIN_LIGHT +14, OUTPUT);
}

void testButtonsStatus(){
  Serial.print("Big=");
  Serial.print(analogRead(PIN_BUTTON_BIG));
  Serial.print("  Red=");
  Serial.print(analogRead(PIN_BUTTON_RED));
  Serial.print("  Black=");
  Serial.print(analogRead(PIN_BUTTON_BLACK));

  Serial.println("");
}

void blinkLight(uint16_t pDurationMS){
  digitalWrite(PIN_LIGHT +14, HIGH);
  delay(pDurationMS);
  digitalWrite(PIN_LIGHT +14, LOW);  
}

void lightSet(boolean pSetOrStop){
  digitalWrite(PIN_LIGHT +14, (pSetOrStop ? HIGH : LOW));
}

//Returns true if button pressed
boolean buttonStatus(byte pButton) {
  //experimentally, if we set level at 800, above means pressed
  return 800 < analogRead(pButton);
}

//returns which button is pressed or 127
uint8_t readTouchKey(){
  if (buttonStatus(PIN_BUTTON_BIG)) return PIN_BUTTON_BIG;
  if (buttonStatus(PIN_BUTTON_RED)) return PIN_BUTTON_RED;
  if (buttonStatus(PIN_BUTTON_BLACK)) return PIN_BUTTON_BLACK;
  
  return 127;
}

#endif __REVEILBUTTONS_H__
