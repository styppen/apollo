/*
 * Valve.h - Library for handling solenoid valves
 * Create by Styppen, March 2017
 */

#include <Arduino.h>
#include <Valve.h>

#define OFF LOW
#define ON HIGH

Valve::Valve(int pin)
{
  pinMode(pin, OUTPUT);
  _valvePin = pin;

	_valveState = LOW;
  _actionState = Valve::STATE_IDLE;
	_previousMillis = 0;
}

void Valve::Update()
{
  // check to see if it's time to change the state of the LED
  unsigned long currentMillis = millis();
  if ((currentMillis - _previousMillis) >= _OnTime)
  {
    if (_actionState == Valve::STATE_FORCESTOP)
    {
      digitalWrite(_valvePin, LOW);
      _actionState = Valve::STATE_IDLE;
    }
    else
    {
      if (_actionState == Valve::STATE_ACTIVE)
      {
        // time is up, time to change state
        Toggle();
        _actionState = Valve::STATE_IDLE;
      }
    }
  }
}

void Valve::TurnOnFor(long period)
{
  if (_actionState != Valve::STATE_ACTIVE)
  {
    _valveState = ON;
    digitalWrite(_valvePin, HIGH);
    _actionState = Valve::STATE_ACTIVE;
    _OnTime = period * 1000; // convert to milliseconds
    _previousMillis = millis();
  }
}

void Valve::ForceStop()
{
  _valveState = OFF;
  _actionState = Valve::STATE_FORCESTOP;
  _previousMillis = 0;
}

void Valve::Toggle()
{
  _valveState = _valveState == ON ? OFF : ON;
  digitalWrite(_valvePin, _valveState);
}
