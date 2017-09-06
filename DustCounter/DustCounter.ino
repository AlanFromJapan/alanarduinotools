/*
 * By Alan
 * See the doc here http://electrogeek.cc/dust%20counter.html
 * based on the below code (see below)
 * 
 Standalone Sketch to use with a Arduino Fio and a
 Sharp Optical Dust Sensor GP2Y1010AU0F
 
 Blog: http://arduinodev.woofex.net/2012/12/01/standalone-sharp-dust-sensor/
 Code: https://github.com/Trefex/arduino-airquality/
 
 For Pin connections, please check the Blog or the github project page
 Authors: Cyrille Médard de Chardon (serialC), Christophe Trefois (Trefex)
 Changelog:
   2012-Dec-01:  Cleaned up code
 
 This work is licensed under the
 Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/
 or send a letter to Creative Commons, 444 Castro Street, Suite 900,
 Mountain View, California, 94041, USA.
*/


//#define _VERBOSE_

int measurePin = 5; //A5
int ledPower = 8;
int ledbarStartPin = 18; //PIN 27 = A4 and we'll go toward the low numbers
 
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
 
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;
 
void setup(){
#ifdef _VERBOSE_
  Serial.begin(9600);
#endif //_VERBOSE_

  pinMode(ledPower,OUTPUT);

  //for the ledbar
  for (int i = 0; i< 10; i++){
    pinMode (ledbarStartPin - i, OUTPUT);
  }

  //for the show
  for (int i = 0; i< 10; i++){
    digitalWrite (ledbarStartPin - i, HIGH);
    delay(50);
  }
  for (int i = 0; i< 10; i++){
    digitalWrite (ledbarStartPin -10 + i, LOW);
    delay(50);
  }
  
}

/* Takes the voltage (in V) read and return the dust density based on the graph provided in the Datasheet
 */
float voltage2DustDensity (float pVoltage) {
  if (pVoltage <= 0.6) {
    return 0;
  }
  if (pVoltage <= 3.4) {
    //linear regression calulated from key values at (v=0.6 dust=0) and (v=3 dust=0.4)
    // x = (y  - b) / a
    return (pVoltage - 0.6)/6.0;  
  }

  //voltage > 3.4 same with [0.5;3.5] and [0.8;3.65]
  // x = (y  - b) / a
  return (pVoltage - 3.25)/0.5;  
}

void loop(){
  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);
 
  voMeasured = analogRead(measurePin); // read the dust value
 
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
 
  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (5.0 / 1024);
 
  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  //dustDensity = 0.17 * calcVoltage - 0.1;
  //above is 1) wrong and 2) for 3.3v
  dustDensity = voltage2DustDensity (calcVoltage);

#ifdef _VERBOSE_

  Serial.print("Raw Signal Value (0-1023): ");
  Serial.print(voMeasured);
 
  Serial.print(" - Voltage: ");
  Serial.print(calcVoltage);
 
  Serial.print(" - Dust Density: ");
  Serial.println(dustDensity);

#endif // _VERBOSE_

  float maxdust = 0.8;
  int vledmax = (int)(10.0 * dustDensity / maxdust);
  /*
  for (int i = 2; i< 12; i++){
    if ((i-2) <= vledmax -1)
      digitalWrite(i,HIGH);
    else
      digitalWrite(i,LOW);
  }
  */
  
  for (int i = 0; i< 10; i++){
    if (i < vledmax )
      digitalWrite(ledbarStartPin - i,HIGH);
    else
      digitalWrite(ledbarStartPin - i,LOW);
  }

 
  delay(500);
}
