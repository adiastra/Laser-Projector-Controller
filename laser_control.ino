#define VERSION "Version 1.3"
 /*
* This is the DiAstra Laser Controler
* It is designed to control safety and sensors for a laser projector
* The ILDA shutter is a relay which is closed (cutting power to the lasers)
* unless the interlock circuit is complete (unbroken)
* and the shutter signal from the software/DAC is set to HIGH (+5v)
* if either of these conditions is false the shutter remains closed
* there is a 5 second delay after closing the shutter before you can open it again
* Written by: Alec DiAstra (alecdiastra@gmail.com)
 */

//Define the type of relay signal required (used for shutterPin)
#define RELAY_SIGNAL LOW

//define the safety delay in milliseconds 
#define SAFETY_DELAY 5000


unsigned long delayTime;
unsigned long repeatMessage;
bool          boot = true;
bool          shutterMessage= false;
bool       interlockMessage = false;
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

   //Has the safety delay been met? 
   if (millis() > delayTime + SAFETY_DELAY) {

        //If the interlock is a circuit AND the shutter is set to 'open'
        if( (digitalRead(interlockIn) == HIGH) && (digitalRead(shutterSignal) == HIGH) )  {             
            // Open the shutter
            digitalWrite(shutterPin, RELAY_SIGNAL);
            laserOn = true;
            if (!shutterMessage) {
              Serial.println("Shutter Open - LASER ARMED!!!!!");
              shutterMessage = true;
              boot = false;
            }
            
        }
        //If interlock curcuit broken
        else if(digitalRead(interlockIn) == LOW)  {
            // Close the shutter
            digitalWrite(shutterPin, !RELAY_SIGNAL);
            Serial.println("Shutter Closed - Interlock Fault");
            shutterMessage = false;
            delayTime = millis();               
        }
        
        //Otherwie the shutter signal is 'closed'
        else  {     
            // Close the shutter
            digitalWrite(shutterPin, !RELAY_SIGNAL);
            Serial.println("Shutter Closed - No shutter signal");
            shutterMessage = false;
            delayTime = millis();
        }

        
   }else{

        
        
        //Close the shutter
        digitalWrite(shutterPin, !RELAY_SIGNAL);
        
        if (millis() > repeatMessage + SAFETY_DELAY) {
          laserOn = true;
          interlockMessage = false;
          
        }
        //If the interlock circuit is broken at boot
        if ((digitalRead(interlockIn) == LOW) && (boot == true))  {
           delayTime = millis();
           Serial.println("Shutter Closed - Interlock Fault");
           boot = false;         
        }

        //If the shutter signal is off at boot (which is totally acceptabl)
        if ((digitalRead(shutterSignal) == LOW) && (boot == true)){
          delayTime = millis();
          Serial.println("Shutter Closed - Ready");
          boot = false; 
        }

        //If the interlock circuit is re-broken during the safety delay period  restart the the delay count
        if (digitalRead(interlockIn) == LOW) {
          delayTime = millis();
          if (!interlockMessage){
            Serial.println("Interlock Fault");
            interlockMessage = true;  
            repeatMessage = delayTime;       
          }
        }

        //If the shutter is re-closed durring the delay period restart the delay count
        if (digitalRead(shutterSignal) == LOW){
          delayTime = millis();          
        }

        //If the last state of the laser was ON let the user know that we are in a safety delay period
        if (laserOn == true){
          Serial.println("Shutter Closed - Safety Delay");
          laserOn = false;
          repeatMessage = delayTime;           
                    
        }

   }
}
 
