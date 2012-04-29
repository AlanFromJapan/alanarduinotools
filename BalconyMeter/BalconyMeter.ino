
byte mSeg7[] = {
   0b01111111
};

void setup()  { 
   for (int i =0; i <= 13; i++){
      pinMode(i, OUTPUT);
   }
   //Serial.begin(9600);
} 

void loop()  { 
   for (int j = 13; j > 9; j--) {
      digitalWrite(13, HIGH);
      digitalWrite(12, HIGH);
      digitalWrite(11, HIGH);
      digitalWrite(10, HIGH);
      
      digitalWrite(j, LOW);

      for (int i = 0; i < 8; i++){
         PORTD = 0x00;
         digitalWrite(i, HIGH);
         delay(500);
      }
   }
}

