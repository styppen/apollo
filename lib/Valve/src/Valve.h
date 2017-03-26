/*
 * Valve.h - Library for handling solenoid valves
 * Create by Styppen, March 2017
 */

#ifndef Valve_h
#define Valve_h

#include <Arduino.h>



class Valve
{
  public:
    // constructors
    Valve(int valvePin, long on, long off);
    Valve(int valvePin);

    // member functions
    void Update();
    void TurnOnFor(long period);
    void TurnOff();
    void Toggle();
    void ForceStop();

    // system state constants
    static const int STATE_IDLE = 0;
    static const int STATE_ACTIVE = 1;
    static const int STATE_FORCESTOP = 2;

  private:
    int _valvePin;
    unsigned long _OnTime;
    long _OffTime;

    int _valveState;
    int _actionState;
    unsigned long _previousMillis;
};

#endif
