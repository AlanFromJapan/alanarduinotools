/*
  AnalogReadSerial
 Reads an analog input on pin 0, prints the result to the serial monitor 
 
 This example code is in the public domain.
 */

#define DEBUG 

void setup() {
  
      //factor settings is to divide internal clock 8MHz by 8.
    //don't, and just run at 8 MHz (set the clock divider to 1 so no effect)
//    CLKPR = (1<<CLKPCE);
//    CLKPR = 0; // Divide by 1 

#ifdef DEBUG
  Serial.begin(9600);
#endif

  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
}



#define CLAP_THRESHOLD 600
#define CLAP_GAP_IN_MS 300
#define CLAP_GAP_TOLERANCE 50

#define CLAP_GAP_MIN (CLAP_GAP_IN_MS - CLAP_GAP_TOLERANCE)
#define CLAP_GAP_MAX (CLAP_GAP_IN_MS + CLAP_GAP_TOLERANCE)

unsigned long mLastClap;
void loop() {
  int sensorValue = analogRead(0);
  if (sensorValue > CLAP_THRESHOLD){
    unsigned long vNow = 0;
    vNow = millis();

    unsigned long vGap = vNow - mLastClap;

#ifdef DEBUG
      Serial.print("clap ! mLastClap = ");
      Serial.print(mLastClap, DEC);
      Serial.print(" ; vNow = ");
      Serial.print(vNow, DEC);
      Serial.print(" ; vGap = ");
      Serial.println(vGap, DEC);
#endif
    
    //blink "clap"
    digitalWrite (6, HIGH);
    delay(50);
    digitalWrite (6, LOW);

    if (
    //initialized
    mLastClap != 0
      //when overflow, last is after current, discard simply
      //and ensure that vGap is signed correctly
    && mLastClap < vNow 
      //within range of duration and tolerance
    && vGap >= CLAP_GAP_MIN && vGap <= CLAP_GAP_MAX
      ){
      //blink "clap-clap"
      digitalWrite (7, HIGH);
      delay(1000);
      digitalWrite (7, LOW);

#ifdef DEBUG
      Serial.print("Calp-clap with delay of: ");
      Serial.println(abs(vNow - mLastClap), DEC);
#endif

      //avoid triple clap to on-off stuffs, reset the last clap
      //would be better with a flag "triggered/dormant" but this will 
      //do the trick in most the cases
      mLastClap = 0;
    }
    else {
      //store last time
      mLastClap = vNow;
    }
  }
}




