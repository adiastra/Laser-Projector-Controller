
 /*
* This is the DiAstra Laser Coltrol
* It is designed to control safety and sensors for a laser projector
* The shutter is a relay which is closed (cutting power to the lasters)
* unless the interlock circuit is complete (unbroken) 
* and the shutter signal from the software/DAC is set to HIGH (+5v)
* if either of these conditions is false the shutter remains closed
* there is a 3 second delay after closing the shutter before you can open it again 
* Written by: Alec DiAstra (alecdiastra@gmail.com)
 */



int shutterSignal = 13;//for reading the shutter signal
int shutterPin = 12;//control relay for shutter
int interlockIn = 5; //interlock Return
int interlockOut = 4; //interlock Send

// setup code for DiAstra laser control
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("DiAstra Laser Control ");
  pinMode(shutterPin, OUTPUT);
  pinMode(interlockIn, INPUT);
  pinMode(interlockOut, OUTPUT);
  pinMode(shutterSignal, INPUT);

}

// loop code DiAstra Laser Controller
void loop() {

//send our interlock signal  
digitalWrite(interlockOut, HIGH);
  
    //The interlock is a circuit and the shutter is set to 'open'
    if ((digitalRead(interlockIn) == HIGH)&&(digitalRead(shutterSignal)) == HIGH){
 
        // Open the shutter
        digitalWrite(shutterPin, LOW);// set shutter pin to low 
        Serial.println("Shutter Open ");
  
    }
       //If interlock curcuit broken
       else if (digitalRead(interlockIn) == LOW){
 
         // Close the shutter
         digitalWrite(shutterPin, HIGH);// set shutter pin to HIGH
         Serial.println("Shutter Closed - Interlock Fault  ");
         delay(3000);
         
         
    }
    
       //Otherwie the shutter signal is 'closed'
       else {
      
        // Close the shutter
        digitalWrite(shutterPin, HIGH);// set shutter pin to HIGH
        Serial.println("Shutter Closed - No shutter signal");
        //wait at lease 3 seconds before you can open the shutter again
        delay(3000);
    
  }
}
 
