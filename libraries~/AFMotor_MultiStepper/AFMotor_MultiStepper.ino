// MultiStepper
// -*- mode: C++ -*-
//
// Control both Stepper motors at the same time with different speeds
// and accelerations. 
// Requires the AFMotor library (https://github.com/adafruit/Adafruit-Motor-Shield-library)
// And AccelStepper with AFMotor support (https://github.com/adafruit/AccelStepper)
// Public domain!

#include <AccelStepper.h>
//#include <AFMotor.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"


// two stepper motors one on each port
//AF_Stepper motor1(200, 1);
//AF_Stepper motor2(200, 2);
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_StepperMotor *motor1 = AFMS.getStepper(200, 1);
Adafruit_StepperMotor *motor2 = AFMS.getStepper(200, 2);


// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// wrappers for the first motor!
void forwardstep1() {  
  motor1->step(1, FORWARD, SINGLE);
}
void backwardstep1() {  
  motor1->step(1, BACKWARD, SINGLE);
}
// wrappers for the second motor!
void forwardstep2() {  
  motor2->step(1, FORWARD, SINGLE);
}
void backwardstep2() {  
  motor2->step(1, BACKWARD, SINGLE);
}

// Motor shield has two motor ports, now we'll wrap them in an AccelStepper object
AccelStepper stepper1(forwardstep1, backwardstep1);
AccelStepper stepper2(forwardstep2, backwardstep2);

void setup()
{  
  
  AFMS.begin();  // create with the default frequency 1.6KHz

  
//  motor1->setSpeed(80);  // 10 rpm   
//  motor2->setSpeed(10);
  
    stepper1.setMaxSpeed(100.0);
    stepper1.setAcceleration(100.0);
    stepper1.moveTo(24);
    
    stepper2.setMaxSpeed(100.0);
    stepper2.setAcceleration(100.0);
    stepper2.moveTo(1000000);
    
}

void loop()
{
  return;
    // Change direction at the limits
    if (stepper1.distanceToGo() == 0)
	stepper1.moveTo(-stepper1.currentPosition());
    stepper1.run();
    stepper2.run();
}
