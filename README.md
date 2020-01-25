# Laser-Projector-Controller
Onboard control for a laser projector

**DISCLAIMER:** This is not for controlling the laser modulation or galvos! 


**SAFETY DISCLAIMER** YOUR are responsable for the safety of your projector. The code provided is designed to make adding a safety curcuit to your system easier. YOU must choose how you want to disable your lasers in an emergency stop situation. YOU must have a protocol in place in these situatations. NEVER leave an active laser projector unattended. 


This is code for an Arduino microcontroller designed to read the current state of the SHUTTER and INTERLOCK from a standard ILDA connector.

The code does the following.

IF the SHUTTER signal is OPEN (+5v) AND the INTERLOCK is a COMPLETE CIRCUIT, only then will aa pin activate that can be used to control whatever shutter method you have chosen (relay on DC power, crobar circuit, grounding modulation) to deactivate your lasers. 

If at any point either of these states changes (shutter closes or interlock is broken) the switch immediately opens (its default un-powered state) and the lasers power is interrupted.

If "shutter" is closed there is a delay before you can turn the power to the lasers back on and the interlock and shutter have to be in the proper states or they will remain off.

If the interlock fails, the :shutter: is closed and remains closed until the interlock issue has been reolved and a safety delay has expired.

Detailed information is printed to serial regarding the state of the system.

*For safety the GALVO power should NEVER be interrupted*

The Arduino pins are as follows

- ShutterSignal = The signal coming from the ILDA shutter signal. +5v = Shutter OPEN (this is an input pin for reading the shutter voltage)
- ShutterPin = Pin used to control the switch state (Output pin for sending signal to the switch)
- InterlockOut = set to +v5 to send a constant signal over the interlock loop (+5v output pin)
- InterlockIn = Reads the signal from the interlock loop sent from the InterlockOut pin(input pin)


# How to use this sketch 
**flash laser_control.ino to the arduino board**

Information on getting started with Arduino can be found here (it's easy!)
https://www.arduino.cc/en/Guide/HomePage


**The shutter**

A circuit should be configured so that the ILDA Shutter pin from the connector (pin 13 from the ILDA DB25) is connected to the shutterSignal pin of the arduino. The shutterPin pin should go to your relay signal pin. the ILDA ground pin (ILDA DB25 pin 25)should be connected to a common ground. 

The he Arduino  needs 5v and can share the same power supply as a relay if needed. Any loss of power to the arduino or switch mechanism should cause the shutter to close. The Arduino board should also share a ground with the ILDA DB25 connector

**The interlock circuit**

The interlockOut pin should connect to pin 4 of ILDA DB25 and the interlockIn pin should connect to pin 17 of the ILDA DB25. This is how the Arduino detects if there is a closed circuit. A 10k resistor should be connected from the interlockIn pin to ground to aleviate any float voltage when the circuit is broken. 

You can use the interlock in a number of ways. 
- enclosure sensors
- safety shutoff button
- anything requiring a circuit to be complete

**Safety delay times**

The delay times for each delay type are configurable at the top of the sketch. note that SHUTTER_DELAY should be the shortest, followed by INTERLOCK_DELAY and BOOT_DELAY should be the longest (all in milliseconds). 

**Different controllers and shutter methods**

If your show controler closes the shutter during the show (for blank periods) you will need to set the SHUTTER_DELAY based on your controllers specs. 

If the SHUTTER_DELAY is low (under 2 seconds) you should NOT use the shutter to control DC power to the lasers as switching the DC power off and on to quickly could damage the lease driver you. Instead use an alternate method for controling the shutter (manual shutter, ground modulation signal). 

MESSAGE_DELAY can be whatever you want as it only controls how often messages repeat in the serial readout. 

**NOTE:** Due to the limitations of the Arduino, the system must be power cycled at minimum every 48 days. I have not come up with a situation where a projector would be active for longer that.
