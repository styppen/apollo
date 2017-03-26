#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Valve.h>
#include <System.h>
#include <Flow.h>
#include <Pump.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

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

// valve control objects
Valve v1(pinV1, Valve::TYPE_NO);
Valve v2(pinV2, Valve::TYPE_NO);
Valve v3(pinV3, Valve::TYPE_NC);

// system control object
System sys(0,0,0,0);

// flow control object
Flow f1(pinF1);

// pump control object
Pump p1(pinP1);

DeviceAddress Probe01 = { 0x28, 0xFF, 0x99, 0xC7, 0x64, 0x15, 0x02, 0x27 };
DeviceAddress Probe05 = { 0x28, 0xFF, 0xD2, 0xAC, 0x64, 0x15, 0x01, 0x58 };
DeviceAddress Probe04 = { 0x28, 0xFF, 0x22, 0xD0, 0x64, 0x15, 0x01, 0x44 };

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

void flow () // Interrupt function
{
   f1.Sample();
}

void transitToState(int futureState)
{

  /**** CURRENT_STATE == FUTURE_STATE ****/
  if (sys.GetState() == futureState)
  {
    // we don't want to do anything if current and future states are the same
    return;
  }

  /**** READY --> PUMPING ****/
  if (sys.GetState() == System::READY && futureState == System::PUMPING)
  {
    // TODO
    Serial.println("READY -> PUMPING");
  }

  /**** READY -> CONSUME ****/
  else if (sys.GetState() == System::READY && futureState == System::CONSUME)
  {
    // TODO
    Serial.println("READY -> CONSUME");
  }

  /**** PUMPING -> READY ****/
  else if (sys.GetState() == System::PUMPING && futureState == System::READY)
  {
    //TODO
    Serial.println("PUMPING -> READY");
  }

  /**** PUMPING -> CONSUME ****/
  else if (sys.GetState() == System::PUMPING && futureState == System::CONSUME)
  {
    //TODO
    Serial.println("PUMPING -> CONSUME");
  }

  /**** CONSUME -> READY ****/
  else if (sys.GetState() == System::CONSUME && futureState == System::READY)
  {
    // TODO
    Serial.println("CONSUME -> READY");
  }

  /**** CONSUME -> PUMPING ****/
  else if (sys.GetState() == System::CONSUME && futureState == System::PUMPING)
  {
    // TODO
    Serial.println("CONSUME -> PUMPING");
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

  lcd.begin(20, 4);
  for(int i = 0; i < 3; i++)
  {
    lcd.backlight();
    delay(100);
    lcd.noBacklight();
    delay(100);
  }
  lcd.backlight(); // finish with backlight on
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

	// ******* 3. process the sensor values and act accordingly *******
	if (flowRate > 0)
	{
		// State::CONSUME mode has a higher priority and must therefore be handled first
		transitToState(System::CONSUME);
	}
	else
	{
		if (temp1 >= 28)
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

  // NOTE: Cursor Position: Lines and Characters start at 0
  lcd.setCursor(0, 0);
  lcd.print("Status: ");
  lcd.setCursor(8, 0);

  if (sys.GetState() == System::READY)
  {
      lcd.print("PRIPRAVLJEN");
  }
  else if (sys.GetState() == System::CONSUME)
  {
    lcd.print("PORABA     ");
  }
  else if(sys.GetState() == System::PUMPING)
  {
    lcd.print("CRPANJE    ");
  }

  lcd.setCursor(0,1); //Start at character 4 on line 0
  lcd.print("T1=");
  lcd.setCursor(3,1);
  lcd.print(temp1);
  lcd.setCursor(9,1);
  lcd.print("C");

  lcd.setCursor(0,2); //Start at character 4 on line 0
  lcd.print("T2=");
  lcd.setCursor(3,2);
  lcd.print(temp2);
  lcd.setCursor(9,2);
  lcd.print("C");

  lcd.setCursor(0,3); //Start at character 4 on line 0
  lcd.print("T3=");
  lcd.setCursor(3,3);
  lcd.print(temp3);
  lcd.setCursor(9,3);
  lcd.print("C");
}
