/*
 TIMINGS are divided by 2 since I'm too lazy to add the board to Arduino IDE AppImage config.
 In case you are a better person than me, you don't need the /2 because Arduino knows that F_CPU is 8MHz and not 16Mhz on a Japanino

 This generates a 300us HIGH every 2ms 
*/
#define TIME_ON_US (300 /2)
#define TIME_OFF_US ((2000-300) /2)

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // change state of the LED by setting the pin to the HIGH voltage level
  delayMicroseconds(TIME_ON_US);
  
  digitalWrite(LED_BUILTIN, LOW);   // change state of the LED by setting the pin to the LOW voltage level
  pinMode(LED_BUILTIN, INPUT);

  delayMicroseconds(TIME_OFF_US);
  pinMode(LED_BUILTIN, OUTPUT);
}
