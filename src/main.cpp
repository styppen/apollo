#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Valve.h>
#include <System.h>
#include <Flow.h>

// threshold temperatures
#define TEMP_X 3

// time constants
#define TIME_T 15

// data wire is plugged into port 2
#define ONE_WIRE_BUS 3

// device configuration
const int pinV1 = 5;
const int pinF1 = 2;

// device state led configuration
const int pinStateV1 = 5;
const int pinStateF1 = 4;

// state led configuration
const int pinStateOff = 1;
const int pinStateIdle = 11;
const int pinStatePumping = 12;
const int pinStateConsume = 13;

// valve control objects
Valve v1(pinV1);
System sys(pinStateOff, pinStateIdle, pinStatePumping, pinStateConsume);
Flow f1(pinF1, pinStateF1);

DeviceAddress Probe01 = { 0x28, 0xFF, 0xD2, 0xAC, 0x64, 0x15, 0x01, 0x58};

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void flow () // Interrupt function
{
   f1.Sample();
}

void setup()
{
	Serial.begin(9600);
	sensors.begin();
	sensors.setResolution(Probe01, 10);

	attachInterrupt(0, flow, RISING);
  sei();
}

void loop()
{
	// prepare inputs for reading
	sensors.requestTemperatures();
	f1.Update();

	// read values from sensors
	float tempC = sensors.getTempC(Probe01);
	int flowRate = f1.GetFlowRate();
	Serial.print("T1=");
	Serial.print(tempC);
	Serial.print("C, flowRate=");
	Serial.print(flowRate);
	Serial.println("L/h");

	// process the sensor values and act accordingly
	if (flowRate > 0)
	{
		// CONSUME mode has a higher priority and must therefore be handled first
		if (sys.GetState() == System::PUMPING)
		{
			v1.ForceStop(); // we need to stop what we are doing right now
		}
		sys.SetState(System::CONSUME);
	}
	else
	{
		if (tempC > 28)
		{
			sys.SetState(System::PUMPING);
			v1.SetStateFor(HIGH, TIME_T);
		}
		else
		{
			if (sys.GetState() == System::PUMPING)
			{
				v1.ForceStop(); // we need to stop what we are doing right now
			}
			sys.SetState(System::IDLE);
		}
	}

	v1.Update();
}
