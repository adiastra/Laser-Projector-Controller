#define VERSION "Version 1.3"
 /*
* This is the DiAstra Laser Coltrol
* It is designed to control safety and sensors for a laser projector
* The shutter is a relay which is closed (cutting power to the lasers)
* unless the interlock circuit is complete (unbroken)
* and the shutter signal from the software/DAC is set to HIGH (+5v)
* if either of these conditions is false the shutter remains closed
* there is a 3 second delay after closing the shutter before you can open it again
* Written by: Alec DiAstra (alecdiastra@gmail.com)
 */

//Define the type of relay signal required
#define RELAY_SIGNAL LOW

//define the safety delay in milliseconds 
#define SAFETY_DELAY 5000


unsigned long delayTime;
bool          boot = true;
bool          shutterMessage= false;
bool          laserOn       = true;
int           shutterSignal = 13;//for reading the shutter signal
int           shutterPin    = 12;//control relay for shutter
int           interlockIn   = 5; //interlock Return
int           interlockOut  = 4; //interlock Send


// setup code for DiAstra laser control
void setup() {
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
    
   if (millis() > delayTime + SAFETY_DELAY) {

        //The interlock is a circuit and the shutter is set to 'open'
        if( (digitalRead(interlockIn) == HIGH) && (digitalRead(shutterSignal) == HIGH) )  {             
            // Open the shutter
            digitalWrite(shutterPin, RELAY_SIGNAL);
            laserOn = true;
            if (!shutterMessage) {
              Serial.println("Shutter Open - LASER ARMED!!!!!");
              shutterMessage = true;
            }
            
        }
        //If interlock curcuit broken
        else if(digitalRead(interlockIn) == LOW)  {
            // Close the shutter
            digitalWrite(shutterPin, !RELAY_SIGNAL);// set shutter pin to HIGH
            Serial.println("Shutter Closed - Interlock Fault");
            shutterMessage = false;
            delayTime = millis();               
        }
        
        //Otherwie the shutter signal is 'closed'
        else  {     
            // Close the shutter
            digitalWrite(shutterPin, !RELAY_SIGNAL);// set shutter pin to HIGH
            Serial.println("Shutter Closed - No shutter signal");
            shutterMessage = false;
            delayTime = millis();
        }
   }
   
   else {
        //Close the shutter
        digitalWrite(shutterPin, HIGH);
        
        if ((digitalRead(interlockIn) == LOW) && (boot == true))  {
           delayTime = millis();
           Serial.println("Shutter Closed - Interlock Fault");
           boot = false;         
        }

        if ((digitalRead(shutterSignal) == LOW) && (boot == true)){
          delayTime = millis();
          Serial.println("Ready");
          boot = false; 
        }

        if (digitalRead(interlockIn) == LOW) {
          delayTime = millis();
        }

        if (digitalRead(shutterSignal) == LOW){
          delayTime = millis();
        }

        if (laserOn == true){
          Serial.println("Shutter Closed - Safety Delay");
          laserOn = false;
                    
        }

        
    
   }
}
 
