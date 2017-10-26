/*
 Fading
 
 This example shows how to fade an LED using the analogWrite() function.
 
 The circuit:
 * LED attached from digital pin 9 to ground.
 
 Created 1 Nov 2008
 By David A. Mellis
 modified 30 Aug 2011
 By Tom Igoe
 
 http://arduino.cc/en/Tutorial/Fading
 
 This example code is in the public domain.
 
 */


int ledPin = 6;    // LED connected to digital pin 9
int ledPin1 = 9;    // LED connected to digital pin 9
int ledPin2 = 10;    // LED connected to digital pin 9
int ledPin3 = 11;    // LED connected to digital pin 9
int maxOff = 50;
int maxOn = 60;


void setup()  { 
  // nothing happens in setup 
  pinMode(2, INPUT_PULLUP);
  pinMode(6, OUTPUT); 
  pinMode(9, OUTPUT); 
  pinMode(10, OUTPUT); 
  pinMode(11, OUTPUT); 
  
  
  pinMode(13, OUTPUT); 
  
  attachInterrupt(0, blink, LOW);
} 

boolean isShooting = false;

 volatile int spinDelay = 100;


void blink()
{
  /*int sensorVal = digitalRead(2);

  if (sensorVal == HIGH) {
    digitalWrite(13, LOW);
  } 
  else {
    digitalWrite(13, HIGH);
  }*/

spinDelay = 10;  
detachInterrupt(0);

/*isShooting = true;
    digitalWrite(ledPin, 1);
    digitalWrite(ledPin1, 1);
    digitalWrite(ledPin2, 1);
    digitalWrite(ledPin3, 1);
    delay(1200);
  
isShooting = false;  
  */
  
  //delay(1200);
  //spinDelay = 100;

}

void loop()  { 

  
  if (!isShooting)
  {
   
  for (int i = 0; i < 10; i++)
  {
    delay(spinDelay);
    digitalWrite(ledPin, 1);
    delay(spinDelay);
    analogWrite(ledPin, 10);         
    digitalWrite(ledPin1, 1);
    delay(spinDelay);
    analogWrite(ledPin1, 3);         
    digitalWrite(ledPin2, 1);
    delay(spinDelay);
    analogWrite(ledPin2, 10);         
    digitalWrite(ledPin3, 1);
    delay(spinDelay);
    analogWrite(ledPin3, 3);         
  }   
  }
  if (spinDelay = 10)
  {
    spinDelay = 100;
    attachInterrupt(0, blink, LOW);
  }
/*
  // fade in from min to max in increments of 5 points:
  for(int fadeValue = maxOff ; fadeValue <= maxOn; fadeValue +=5) { 
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);         
    analogWrite(ledPin1, maxOn-fadeValue);         
    analogWrite(ledPin2, fadeValue);         
    analogWrite(ledPin3, maxOn-fadeValue);         
    
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);                            
  } 

  // fade out from max to min in increments of 5 points:
  for(int fadeValue = maxOn ; fadeValue >= maxOff; fadeValue -=5) { 
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);         
    analogWrite(ledPin1, maxOn-fadeValue);         
    analogWrite(ledPin2, fadeValue);         
    analogWrite(ledPin3, maxOn-fadeValue);         
    
    // wait for 30 milliseconds to see the dimming effect    
    delay(30);                            
  } 
*/ 
}


