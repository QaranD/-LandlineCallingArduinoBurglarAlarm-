
#include "Arduino.h"
#include "DTMFDialer.h"
#include <RCSwitch.h>
//In this sketch devise support a wire sensor and a wireless sensor and if receive signal from them, it turns the alarm on and makes telephone call with telephone line
const byte tone1Pin = 12; // pin for tone 1
const byte tone2Pin = 13; // pin for tone 2
byte PhoneNumber1[] = {0, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // for special characters: 10=*, 11=#, 12=1sec delay
byte PhoneNumber2[] = {0, 9, 2, 2, 2, 2, 2, 2, 2, 2, 2}; // for special characters: 10=*, 11=#, 12=1sec delay
byte PhoneNumber3[] = {1,1,1,1,1,1,1,0};
int Motionsensor = 4; //motion detector
byte PhoneNumberLength = 11;
byte PhoneNumber3Length = 8;
int relay = 8;
int AlarmPin = 5;
bool nchg = 0;

long unsigned int InactiveSignal1 = 1234567;
long unsigned int InactiveSignal2 = 2345678;
long unsigned int ActiveSignal1 = 3456789;
long unsigned int ActiveSignal2 = 4567890;
long unsigned int MotionSignal = 5678901;

unsigned long CurrentTime;

unsigned long PhoneCallstartTime;
unsigned long AlarmstartTime;
//unsigned long PhoneCallCurrentTime;

RCSwitch mySwitch = RCSwitch();
bool isactive = true;
bool motiondetect = false;
uint8_t st = 0;
uint8_t countcall = 0;
bool frstloop = true;
uint8_t WLmotion = 0;

DTMFDialer _DTMFDialer(tone1Pin, tone2Pin);

void setup()
{
    //Serial.begin(9600);

  pinMode(relay, OUTPUT);
  pinMode(Motionsensor, INPUT);
  pinMode(AlarmPin, OUTPUT);
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2

}

void loop()
{

  if (mySwitch.available()) {

    int value = mySwitch.getReceivedValue();

    if (value == 0) {
    } else {
      long unsigned int a = mySwitch.getReceivedValue() ;
      //Serial.println(a);

      if (a == InactiveSignal1 || a == InactiveSignal2)
      {
        motiondetect = false;
        isactive = false;
        st = 0;
        countcall = 0;
        WLmotion = 0;
      }
      if (a == ActiveSignal1 || a == ActiveSignal2 )
      {
        digitalWrite(AlarmPin, LOW);
        isactive = true;
        st = 0;
        countcall = 0;
        mySwitch.resetAvailable();
        motiondetect = false;
        delay(4000);
        motiondetect = false;
        WLmotion = 0;


      }
      if (a == MotionSignal )
      {
        WLmotion = 1;
      }
    }
  }
  
  int motion = digitalRead(Motionsensor);   // read sensor value
  //Serial.println(motion);

  if (frstloop == true)
  {
    delay(10000);
    //motion=LOW;
    frstloop = false;
    digitalWrite(AlarmPin, LOW);
    motion = 0;
    WLmotion = 0;
  }
  if (isactive == true)
  {
    if (WLmotion == 1 || motion == 1  )
    {
      //Serial.println(WLmotion);

      digitalWrite(AlarmPin, HIGH);
      motiondetect = true;
      if (st == 0) {
        PhoneCallstartTime = millis();
        AlarmstartTime = millis();
        st = 1;
      }

    }
  }
  if (motiondetect == false) {
    digitalWrite(AlarmPin, LOW);
    motion=0;
  }
    //Serial.println(motiondetect);


  CurrentTime = millis();
  if (motiondetect == true && isactive == true && (CurrentTime - AlarmstartTime) > 180000)
  {
    digitalWrite(AlarmPin, LOW);
    delay(300000);
    AlarmstartTime = millis();
    WLmotion = 0;
    motion = LOW;
    motiondetect = false;
  }



  if (motiondetect == true && isactive == true && (CurrentTime - PhoneCallstartTime) > 60000)
  {

    if (nchg == 0)
    {
      digitalWrite(relay, HIGH);
      delay(3000);
      _DTMFDialer.dialNumber(PhoneNumber1, PhoneNumberLength); // Dial the number
      delay(15000);
      digitalWrite(relay, LOW);
      delay(15000);
      nchg = 1;
      countcall++;
    } else if(nchg==1)
    {
      digitalWrite(relay, HIGH);
      delay(3000);
      _DTMFDialer.dialNumber(PhoneNumber2, PhoneNumberLength); // Dial the number
      delay(15000);
      digitalWrite(relay, LOW);
      delay(15000);
      nchg = 2;
      countcall++;
    }
    else{
      digitalWrite(relay, HIGH);
      delay(3000);
      _DTMFDialer.dialNumber(PhoneNumber3, PhoneNumber3Length); // Dial the number
      delay(15000);
      digitalWrite(relay, LOW);
      delay(15000);
      nchg = 0;
      countcall++;
      }
    

  }

  if (countcall > 5)
  {
    PhoneCallstartTime = millis();
    countcall = 0;
  }


}
