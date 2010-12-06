#include "WProgram.h"
#include "wiring.h"

#include "TouchScreen.h"


//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
//Define the I/O pin we will use for our sound output
#define SOUNDOUT_PIN 8
#define WHOLE_NOTE_TIME 1000
//from C1
int mNotes [] = {
  65,73,82,87,98,110,123,131,147,165,175,196,220,247,262,294,330,349,392,440,494,523,587,659,698,784,880,988,1047,1175,1319,1397,1568,1760,1976,2093,2349,2637,2794,3136,3520,3951,4186,4699 };
#define NOTES_ARRAY_LEN 44
char tididit[] = "e58f58g58  4g58"; //used for saying ok!
//http://gprime.net/images/mariopiano/
char mario3[] = "e48e44e44c48e44g44  2" "c32  8g24  8e22  8" "e24  sa32  sb32  sb34  sa32"
"c34e44g44a54f48g58" "e44c48d48b43" "c43g34  4e22"
;
char clairefont2[] = "g32g34b34" "b34a34b34b34" "g32g34b34" "b34a34b32"
"b32b34a34" "g34b34c44b34" "c42c44b34" "g34b34a32"
"g32g34b34" "b34a38g38b34g34" "b32b34a38g38" "b34a34g32"
;  
char furElise1[] = "e44d44" "e44d44e44b34d44c44" "a32  2a34" "b32  2g34b34" "c42"
;
char nothingElse1[] = "e32g32b32e42b32g32" "e32g32b32e42b32g32" "e32g32b32e42b42" "b54b51b58e41" 
"b54c64b54a54b54a54e41" "b32  8b32e42b32f48  8e42" "e42b32e31"
;
#ifdef REVEIL_DEBUG
char* playlist[] = {
  tididit}; 
#define PLAYLIST_LEN 1
#else
char* playlist[] = {
  clairefont2, furElise1, nothingElse1, mario3}; 
#define PLAYLIST_LEN 4
#endif




//http://www.uchobby.com/index.php/2007/11/22/arduino-sound-part-3-playing-a-melody/
void Sound(float freq, int durationMS, int outputPin) {
  int halfPeriod;
  float period;
  int durationCycles;
  //Check for rest, 0 frequency is a rest for durationMS.
  if(freq==0.0) {
    //0 frequency so we stay quiet for duration
    delay (durationMS);
  }
  else { //Frequency is not zero so we have work to do
    // turn on output pin
    pinMode(outputPin, OUTPUT);
    //calculate the period or cycle time for the given frequency
    period=1/freq; //Take the reciprocal to get time in seconds
    period=period*1.0E6; //to covert seconds to uS.
    //divide that by 2 to get the 1/2 cycle time. convert to int at the same time
    halfPeriod = (int)(period/2.0) - 7; // subtract 7 us to make up for digitalWrite overhead

    // calculate cycles for duration.
    durationCycles = (int)(((float)durationMS*1000.0)/period); // play note for duration ms
    for (int i=0; i<durationCycles; i++){
      digitalWrite(outputPin, HIGH);
      delayMicroseconds(halfPeriod);
      digitalWrite(outputPin, LOW);
      delayMicroseconds(halfPeriod - 1); // - 1 to make up for fractional microsecond                                              in digitaWrite overhead
    }
    // shut off pin to avoid noise from other operations
    pinMode(outputPin, INPUT);
  }
}


//http://www.uchobby.com/index.php/2007/11/22/arduino-sound-part-3-playing-a-melody/
//returns true if left by touch, otherwise false
boolean PlayMusicString(char* music, int outputPin){
  int noteCount=strlen(music);
  float freq;
  int duration;
  for (int i=0;i<noteCount;i+=3) {
    if (music[i] == ' '){
      freq=0;
    }
    else {
      byte vNote = 0; //C
      switch(music[i]){
      case 'c' :
        vNote =0;
        break;
      case 'd' :
        vNote =1;
        break;
      case 'e' :
        vNote =2;
        break;
      case 'f' :
        vNote =3;
        break;
      case 'g' :
        vNote =4;
        break;
      case 'a' :
        vNote =5;
        break;
      case 'b' :
        vNote =6;
        break;	
      }
      byte vOctave = music[i+1] - '1';
      if (vNote + 7 * vOctave < NOTES_ARRAY_LEN){
        freq = mNotes[vNote + 7 * vOctave];
      }
      else {
        //note unknown
        freq = 0;
      }
    }
    //Note Timing
    switch(music[i+2]){
    case '1' :
      duration=WHOLE_NOTE_TIME;
      break;
    case '2' :
      duration=WHOLE_NOTE_TIME/2;
      break;
    case '3' :
      duration=WHOLE_NOTE_TIME/3;
      break;
    case '4' :
      duration=WHOLE_NOTE_TIME/4;
      break;
    case '6' :
      duration=WHOLE_NOTE_TIME/6;
      break;
    case '8' :
      duration=WHOLE_NOTE_TIME/8;
      break;
    case 's' :
      duration=WHOLE_NOTE_TIME/16;
      break;
    }
    Sound(freq,duration,outputPin);

    delay(WHOLE_NOTE_TIME/32
      - 30 //estimated time for reading the touch 
    );

    //is touched ?
    int x;
    int y;
    if (readTouch(x, y)){
      //leave
      return true;
    }
  }
  return false;
}
