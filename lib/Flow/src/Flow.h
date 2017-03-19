#ifndef Flow_h
#define Flow_h

#include <Arduino.h>

class Flow
{
  public:
    //constructor
    Flow(int flowPin, int flowLedPin);

    //member functions
    void Sample();
    void Update();
    boolean isFlowing();
    int GetFlowRate();

  private:
    int _flowPin;
    int _flowLedPin;
    volatile int _flowFrequency; // Measures flow sensor pulses
    unsigned int _flowRate; // Calculated litres/hour
    unsigned long _currentTime;
    unsigned long _cloopTime;

};

#endif
