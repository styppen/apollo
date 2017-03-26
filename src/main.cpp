#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Valve.h>
#include <System.h>
#include <Flow.h>
#include <Pump.h>

// threshold temperatures
#define TEMP_X 3

// time constants
#define TIME_T 15

// data wire is plugged into port 2
#define ONE_WIRE_BUS 3

// device configuration
const int pinF1 = 2;
const int pinV1 = 5;
const int pinV2 = 6;
const int pinV3 = 7;
const int pinP1 = 8;

// device state led configuration
const int pinStateV1 = 5;
const int pinStateF1 = 4;

// state led configuration
const int pinStateOff = 1;
const int pinStateIdle = 11;
const int pinStatePumping = 12;
const int pinStateConsume = 13;

// valve control objects
Valve v1(pinV1, Valve::TYPE_NO);
Valve v2(pinV2, Valve::TYPE_NO);
Valve v3(pinV3, Valve::TYPE_NC);

// system control object
System sys(pinStateOff, pinStateIdle, pinStatePumping, pinStateConsume);

// flow control object
Flow f1(pinF1, pinStateF1);

// pump control object
Pump p1(pinP1);

DeviceAddress Probe01 = { 0x28, 0xFF, 0x99, 0xC7, 0x64, 0x15, 0x02, 0x27 };
DeviceAddress Probe05 = { 0x28, 0xFF, 0xD2, 0xAC, 0x64, 0x15, 0x01, 0x58 };
DeviceAddress Probe04 = { 0x28, 0xFF, 0x22, 0xD0, 0x64, 0x15, 0x01, 0x44 };

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void flow () // Interrupt function
{
   f1.Sample();
}

void transitToState(int futureState)
{

  if (sys.GetState() == futureState)
  {
    // state won't change, we don't do anything
    return;
  }
  else if (sys.GetState() == System::PUMPING)
  {
    //we have to stop pumping
  }
  else if (futureState == System::PUMPING)
  {
    // start the pumping routine
  }
  sys.SetState(futureState);

}

void setup()
{
	Serial.begin(9600);
	sensors.begin();
  sensors.setResolution(Probe01, 10);
	sensors.setResolution(Probe05, 10);
  sensors.setResolution(Probe04, 10);

	attachInterrupt(0, flow, RISING);
  sei();
}

void loop()
{
	// ******* 1. prepare inputs for reading *******
	sensors.requestTemperatures();
	f1.Update();

	// ******* 2. read values from sensors *******
	float temp1 = sensors.getTempC(Probe05);
  float temp2 = sensors.getTempC(Probe04);
  float temp3 = sensors.getTempC(Probe01);
	int flowRate = f1.GetFlowRate();
	Serial.print("T1=");
	Serial.print(temp1);
  Serial.print("C, T2=");
  Serial.print(temp2);
  Serial.print("C, T3=");
  Serial.print(temp3);
	Serial.print("C, flowRate=");
	Serial.print(flowRate);
	Serial.println("L/h");

	// ******* 3. process the sensor values and act accordingly *******
	if (flowRate > 0)
	{
		// State::CONSUME mode has a higher priority and must therefore be handled first
		transitToState(System::CONSUME);
	}
	else
	{
		if (temp1 > 28)
		{
      //temperature difference is high than threshold, we initiate pumping
			transitToState(System::PUMPING);
		}
		else
		{
			transitToState(System::READY);
			sys.SetState(System::READY);
		}
	}

  p1.Update();
}
