/*
  ardClapClap
  Clap twice, it triggers on/off a relay and whatever is plugged on.
 http://kalshagar.wikispaces.com/Clapclap
 
 This code is in the public domain.
 */

#define DEBUG 

#define LED_PIN_CLAP 6
#define LED_PIN_CLAPCLAP 7
#define RELAY_PIN 8

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif

  pinMode(LED_PIN_CLAPCLAP, OUTPUT);
  pinMode(LED_PIN_CLAP, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
}

#define CLAP_THRESHOLD 600
#define CLAP_GAP_IN_MS 300
#define CLAP_GAP_TOLERANCE 65

#define CLAP_GAP_MIN (CLAP_GAP_IN_MS - CLAP_GAP_TOLERANCE)
#define CLAP_GAP_MAX (CLAP_GAP_IN_MS + CLAP_GAP_TOLERANCE)

unsigned long mLastClap;
boolean mRelayStatus = false;

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
    digitalWrite (LED_PIN_CLAP, HIGH);
    delay(50);
    digitalWrite (LED_PIN_CLAP, LOW);

    if (
    //initialized
    mLastClap != 0
      //when overflow, last is after current, discard simply
    //and ensure that vGap is signed correctly
    && mLastClap < vNow 
      //within range of duration and tolerance
    && vGap >= CLAP_GAP_MIN && vGap <= CLAP_GAP_MAX
      ){

      //switch relay status
      mRelayStatus = !mRelayStatus;
      digitalWrite(RELAY_PIN, (mRelayStatus ? HIGH : LOW));

      //blink "clap-clap"
      digitalWrite (LED_PIN_CLAPCLAP, HIGH);
      delay(500);
      digitalWrite (LED_PIN_CLAPCLAP, LOW);

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





