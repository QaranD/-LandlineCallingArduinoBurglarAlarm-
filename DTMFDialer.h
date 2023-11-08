#ifndef DTMFDialer_h
#define DTMFDialer_h

#include "Arduino.h"

class DTMFDialer
{
  public:
    DTMFDialer( byte tone1Pin, byte tone2Pin);
    void dialNumber(byte number[],byte len);
    void playDTMF(byte digit, byte duration);
        
  private:
    byte _len;
    byte _number[];
    byte _tone1Pin;
    byte _tone2Pin;
};

#endif