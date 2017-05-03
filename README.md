# Apollo

Apollo is a project based on an Arduino micro controller responsible for pumping hot water.

## Idea
![Alt text](http://i.imgur.com/ZSUKzkP.png, "System overview")

The main culprits are two water reservoirs. One boiler (B1) is located inside a
bathroom and the second one (B2) is located on the roof. The upper boiler is
being heated by the sun and when the temperature difference between B1 and B2
reaches certain threshold, the pump is activated and the hot water is transferred
from B2 to B1.

Via the water flow sensor, the system is able to detect the water flow in case a
water was opened. When such flow is detected we should terminate any potential
pumping that was taking place at that time.

## Hardware
The system consists of the following components:
* Micro Controller Unit (Arduino Uno)
* OneWire temperature sensors (the waterproof ones)
* Hall effect water/gas flow sensor (the brass one)
* Relay module (with 4 ... erm ... relays)
* 3 220V solenoid valves (2 Normally Open, 1 Normally Closed)
* 220V water pump

## Schematics
![Alt text](http://imgur.com/VkQkoCE.png, "Schematic")
