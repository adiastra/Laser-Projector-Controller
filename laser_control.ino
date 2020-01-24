
#define VERSION "Version 1.7"
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
//BOOT should be the longest then INTERLOCK with SHUTTER being the shortest
#define SHUTTER_DELAY 5000 
#define INTERLOCK_DELAY 10000
#define BOOT_DELAY 20000

//How long before messages repeat.
#define MESSAGE_DELAY 5000

unsigned long delayTime;
unsigned long repeatMessage;
bool boot = true;
bool laserMessage = false;
bool shutterMessage = false;
bool interlockMessage = false;
bool shutterReady = false;
bool interlock = LOW; 
bool shutter = LOW; 
//Pins can be any digital pins
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
  
  //Startup delay
  if (boot)
  {
    digitalWrite(shutterPin, !RELAY_SIGNAL);
    Serial.println("BOOT DELAY");
    delay(BOOT_DELAY);
    boot = false;
    Serial.println("BOOT COMPLETE");
  }
  

}//END SETUP

// loop code DiAstra Laser Controller
void loop()
{
  //always send interlock signal  
  digitalWrite(interlockOut, HIGH);
  checkPins();

  if (millis() >= delayTime + INTERLOCK_DELAY)
  {
    while((shutter) && (interlock))
    {
      digitalWrite(shutterPin, RELAY_SIGNAL);
      checkPins();
      shutterMessage = false;
      interlockMessage = false;
      shutterReady = false;
      if (!laserMessage)
      {
        Serial.println("Shutter Open - !!! LASER ON !!!");
        laserMessage = true;    }
    }
  }
  
  if (!shutter)
  {
    digitalWrite(shutterPin, !RELAY_SIGNAL);
    laserMessage = false;
    if (!shutterMessage)
    {
      if (!shutterReady)
      {
        Serial.println("Shutter Closed - Safety Delay");
        delay(SHUTTER_DELAY);
        shutterReady = true;
      }
      else
      {
        Serial.println("Shutter Closed - Laser Ready");
        shutterMessage = true;  
        repeatMessage = millis();    
      }
    }
  }

  if (!interlock)
  {
    digitalWrite(shutterPin, !RELAY_SIGNAL);    
    laserMessage = false;
    delayTime = millis();
    if (!interlockMessage)
    {
      Serial.println("!!! INTERLOCK FAULT !!!");
      interlockMessage = true;
      repeatMessage = millis();
      
    }    
  }

  if (millis() >= repeatMessage + MESSAGE_DELAY)
  {
    shutterMessage = false;
    interlockMessage = false;
  }
}  // END LOOP
  
  



//Function for checking the state of the sensing pins
void checkPins()
{
 if (interlock)
 {
  shutter = digitalRead(shutterSignal);
 }
 else
 {
  shutterReady = false;  
 } 
 
 interlock = digitalRead(interlockIn);
}
