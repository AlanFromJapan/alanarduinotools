//AlanFromJapan
//http://kalshagar.wikispaces.com/ardServerRoomProbe

#define PIN_LED 12
 
#define SENSOR_COUNT 3
#define SENSOR_AVERAGE 6
 
long mSavedValues[SENSOR_COUNT][SENSOR_AVERAGE];
int mRound = 0;
 
void setup()                    // run once, when the sketch starts
{
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(9600); // begin serial
 
  //init table of values
  for (int i =0; i < SENSOR_COUNT; i++){
    for (int j = 0; j < SENSOR_AVERAGE; j++){
      mSavedValues[i][j] = 0;
    }
  }
}
 
long vVal = 0;
long vValScaled = 0;
char vMessage[50];
char vBuffer[20];
 
void loop()                     // run over and over again
{
 
  //start reading
  digitalWrite(PIN_LED, HIGH);
 
  //read values and store
  for (int i =0; i < SENSOR_COUNT; i++){
    vVal = analogRead(i);
    vValScaled = ((long)5*vVal*(long)100/(long)1024);         //convert voltage to temperature
    mSavedValues[i][mRound] = vValScaled;
  }
 
  mRound = (mRound + 1) % SENSOR_AVERAGE;
 
 
  //calculate average
  for (int i =0; i < SENSOR_COUNT; i++){
    //reset message buffer
    vMessage[0] = 0;
    vVal = 0;
    for (int j = 0; j < SENSOR_AVERAGE; j++){
      vVal += mSavedValues[i][j];
    }
    vVal = vVal / SENSOR_AVERAGE;
 
    itoa(i, vBuffer, 10);
    strcat(vMessage,vBuffer);
    strcat(vMessage,"=");
    itoa(vVal, vBuffer, 10);
    strcat(vMessage,vBuffer);
    strcat(vMessage,",");
 
    Serial.print(vMessage);
  }
  Serial.println("");
 
  //end of read
  digitalWrite(PIN_LED, LOW);
 
 
  delay(3000);
}
