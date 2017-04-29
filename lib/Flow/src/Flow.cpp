#include <Arduino.h>
#include <Flow.h>

Flow::Flow(int flowPin, int flowLedPin)
{
  _flowPin = flowPin;
  _flowLedPin = flowLedPin;
  digitalWrite(_flowPin, HIGH);
  _currentTime = millis();
  _cloopTime = _currentTime;
}

Flow::Flow(int flowPin)
{
  _flowPin = flowPin;
  digitalWrite(_flowPin, HIGH);
  _currentTime = millis();
  _cloopTime = _currentTime;
}

void Flow::Sample()
{
  _flowFrequency++;
}

void Flow::Update()
{
  _currentTime = millis();
  if(_currentTime >= (_cloopTime + 1500))
  {
    _cloopTime = _currentTime;
    _flowRate = (_flowFrequency * 60 / 7.5);
    _flowFrequency = 0;
  }
}

boolean Flow::isFlowing()
{
  return _flowRate > 0;
}

int Flow::GetFlowRate()
{
  return _flowRate;
}
