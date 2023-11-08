#include "Arduino.h"
#include "DTMFDialer.h"

int DTMF[13][2]={
  {941,1336}, // frequencies for touch tone 0
  {697,1209}, // frequencies for touch tone 1
  {697,1336}, // frequencies for touch tone 2
  {697,1477}, // frequencies for touch tone 3
  {770,1209}, // frequencies for touch tone 4
  {770,1336}, // frequencies for touch tone 5
  {770,1477}, // frequencies for touch tone 6
  {852,1209}, // frequencies for touch tone 7
  {852,1336}, // frequencies for touch tone 8
  {852,1477}, // frequencies for touch tone 9
  {941,1209}, // frequencies for touch tone *
  {941,1477}, // frequencies for touch tone #
  {0,0} // pause
};

DTMFDialer::DTMFDialer( byte tone1Pin, byte tone2Pin)
{
  pinMode(tone1Pin, OUTPUT);
  pinMode(tone2Pin, OUTPUT);
  _tone1Pin = tone1Pin;
  _tone2Pin = tone2Pin;

}

void DTMFDialer:: dialNumber(byte number[],byte len)
{
   for(int i=0;i<len;i++)
   {
    playDTMF(number[i], 100);  // 100 msec duration of tone
    delay(100); // 100 msec pause between tones
   }
}

void DTMFDialer::playDTMF(byte digit, byte duration)
{
  boolean tone1state=false;
  boolean tone2state=false;
  int tone1delay=(500000/DTMF[digit][0])-10;
  int tone2delay=(500000/DTMF[digit][1])-10; 
  unsigned long tone1timer=micros();
  unsigned long tone2timer=micros();
  unsigned long timer=millis(); 
  if(digit==12){
    delay(1000); // one second delay if digit is 12
  } else {
    while(millis()-timer<duration){
      if(micros()-tone1timer>tone1delay){
        tone1timer=micros(); // reset the timer
        tone1state=!tone1state; // toggle tone1state
        digitalWrite(_tone1Pin, tone1state);
      }
      if(micros()-tone2timer>tone2delay){
        tone2timer=micros(); // reset the timer
        tone2state=!tone2state; // toggle tone2state
        digitalWrite(_tone2Pin, tone2state);
      }
    }
    digitalWrite(_tone1Pin,LOW);
    digitalWrite(_tone2Pin,LOW);
  }
}
