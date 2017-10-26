/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438

This sketch creates a fun motor party on your desk *whiirrr*
Connect a unipolar/bipolar stepper to M3/M4
Connect a DC motor to M1
Connect a hobby servo to SERVO1
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
#include <Servo.h> 

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 


// We'll also test out the built in Arduino Servo library
Servo servo1;
Servo servo2;


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("MMMMotor party!");

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  // Attach a servo to pin #10
  servo1.attach(10);
  servo2.attach(9);
  
  servo2.write(map(0, 0, 255, 0, 180));
    
  // turn on motor M1
}

int i;
void loop() {
  
  return;
  for (i=0; i<255; i++) {
    servo1.write(map(i, 0, 255, 0, 180));
    servo2.write(map(i/3+150, 0, 255, 0, 180));
    
    delay(30);
 }
 
 for (i=255; i!=0; i--) {
    servo1.write(map(i, 0, 255, 0, 180));
    servo2.write(map(i/3+150, 0, 255, 0, 180));

    delay(3);
 }
 /*
  for (i=0; i<255; i++) {
    servo1.write(map(i, 0, 255, 0, 180));
    servo2.write(map(i, 0, 255, 0, 180));

    delay(3);
 }
 
  for (i=255; i!=0; i--) {
    servo1.write(map(i, 0, 255, 0, 180));
    servo2.write(map(i, 0, 255, 0, 180));
    
    delay(3);
 }*/
}
