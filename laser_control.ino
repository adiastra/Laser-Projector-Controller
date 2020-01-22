
#define VERSION "Version 1.5"
/*
 *This is the DiAstra Laser Controler
 *It is designed to control safety and sensors for a laser projector
 *The ILDA shutter is a relay which is closed (cutting power to the lasers)
 *unless the interlock circuit is complete (unbroken)
 *and the shutter signal from the software/DAC is set to HIGH (+5v)
 *if either of these conditions is false the shutter remains closed
 *there is a 5 second delay after closing the shutter before you can open it again
 *Written by: Alec DiAstra (alecdiastra@gmail.com)
 */

//Define the type of relay signal required (used for shutterPin)
#define RELAY_SIGNAL LOW

//define the safety delay in milliseconds 
#define SAFETY_DELAY 5000

unsigned long delayTime;
unsigned long repeatMessage;
bool boot = true;
bool laserMessage = false;
bool shutterMessage = false;
bool interlockMessage = false;
bool safetyMessage = true;
int shutterSignal = 13;  //for reading the shutter signal
int shutterPin = 12;  //control relay for shutter
int interlockIn = 5;  //interlock Return
int interlockOut = 4; //interlock Send

// setup code for DiAstra laser control
void setup()
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("DiAstra Laser Control");
  Serial.println(VERSION);

  //Initialize the required pins
  pinMode(shutterPin, OUTPUT);
  pinMode(interlockIn, INPUT);
  pinMode(interlockOut, OUTPUT);
  pinMode(shutterSignal, INPUT);
  delayTime = millis();

}

// loop code DiAstra Laser Controller
void loop()
{
  //send our interlock signal  
  digitalWrite(interlockOut, HIGH);

  if (millis() > (delayTime + SAFETY_DELAY))
  {
    //Has SAFETY_DELAY been met?

     //If the interlock is a circuit AND the shutter is set to 'open'
    if ((digitalRead(interlockIn) == HIGH) && (digitalRead(shutterSignal) == HIGH))
    {
      // Open the shutter
      digitalWrite(shutterPin, RELAY_SIGNAL);
      interlockMessage = false;
      shutterMessage = false;
      safetyMessage = false;
      if (!laserMessage)
      {
        Serial.println("Shutter Open - LASER ARMED!!!!!");
        laserMessage = true;
      }
    }
    else
    {
      //else turn of the laser
      digitalWrite(shutterPin, !RELAY_SIGNAL);
      delayTime = millis();
      laserMessage = false;
    }
  }
  else
  {
    //SAFETY_DELAY has not been met

    //keep the shutter closed
    digitalWrite(shutterPin, !RELAY_SIGNAL);

    //reset the delay timer if either condition changes
    if ((digitalRead(shutterSignal) == LOW) || (digitalRead(interlockIn) == LOW))
    {
      delayTime = millis();
    }

    //If interlock curcuit broken
    if (digitalRead(interlockIn) == LOW)
    {
      if (!interlockMessage)
      {
        Serial.println("Shutter Closed - Interlock Fault");
        interlockMessage = true;
        repeatMessage = millis();
      }
    }

    // If shitter is closed
    if (digitalRead(shutterSignal) == LOW)
    {
      if (!shutterMessage)
      {
        Serial.println("Shutter Closed - No Shutter Signal");
        shutterMessage = true;
        repeatMessage = millis();
      }
    }

    if (!safetyMessage)
    {
      Serial.println("Shutter Closed - Safety Delay");
      safetyMessage = true;
      repeatMessage = delayTime;
      laserMessage = false;
    }

    if (boot)
    {
      Serial.println("Boot Delay");
      delay(SAFETY_DELAY);
      boot = false;
    }

    if (millis() > (repeatMessage + SAFETY_DELAY))
    {
      shutterMessage = false;
      interlockMessage = false;
    }
  }
}
