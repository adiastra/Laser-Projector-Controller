# Laser-Projector-Controller
Onboard control for a laser projector

DISCLAIMER: This is not for controlling the laser modulation or galvos!


This is code for an Arduino microcontroller designed to read the current state of the SHUTTER and INTERLOCK from a standard ILDA connector.

The code does the following.

IF the SHUTTER signal is OPEN (+5v) AND the INTERLOCK is a COMPLETE CIRCUIT, only then will a relay close providing power to the laser(s). (after a 5 second delay)

If at any point either of these states changes (shutter closes or interlock is broken) the relay immediately opens (its default un-powered state) and the lasers power is interrupted.

If laser power is interrupted there is a 5 second delay before you can turn the power to the lasers back on and the interlock and shutter have to be in the proper states or they will remain off.

The current state of the laser power is printed to serial if you want to monitor the state.

*For safety the GALVO power should NEVER be interrupted*

The Arduino pins are as follows

- ShutterSignal = The signal coming from the ILDA shutter signal. +5v = Shutter OPEN (this is an input pin for reading the shutter voltage)
- ShutterPin = Pin used to control the relay state (Output pin for sending signal to the relay
- InterlockOut = set to +v5 to send a constant signal over the interlock loop (+5v output pin)
- InterlockIn = Reads the signal from the interlock loop sent from the InterlockOut pin(input pin)


# How to use this sketch 
**flash laser_control.ino to the arduino board**

A circuit should be configured so that the ILDA Shutter pin from the connector (pin 13 on the DB25) is connected to the shutterSignal pin of the arduino. The shutterPin pin should go to your relay signal pin. The relay and the Arduino both need 5v and can share the same power supply. Any loss of power to the arduino or relay will cause the shutter to close. 

The Arduino board should share a ground with the DB25 connector

**The interlock circuit**

The interlockOut pin should connect to pin 4 of ILDA DB25 and the interlockIn pin should connect to pin 17 of the ILDA DB25. This is how the Arduino detects if there is a closed circuit. A 10k resistor can be used on these pins connected to ground to aleviate any float voltage when the circuit is broken. 

You can use the interlock in a number of ways. 
- enclosure sensors
- safety shutoff button
- anything requiring a circuit to be complete

**Safety delay times**

The delay times for each delay type are configurable at the top of the sketch. note that SHUTTER_DELAY should be the shortest, followed by INTERLOCK_DELAY and BOOT_DELAY should be the longest (all in milliseconds). 

If your show controler closes the shutter during 

MESSAGE_DELAY can be whatever you want as it only controls how often messages repeat in the serial readout. 
