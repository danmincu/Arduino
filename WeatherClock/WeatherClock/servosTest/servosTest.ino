// this sketch cycles three servos at different rates 

#include <ServoTimer2.h>  // the servo library

// define the pins for the servos
#define rollPin  A0             
#define pitchPin A1
#define yawPin   4

ServoTimer2 servoRoll;    // declare variables for up to eight servos
ServoTimer2 servoPitch; 
ServoTimer2 servoYaw; 
 
void setup() {
  
   Serial.begin(9600);
  //servoRoll.attach(rollPin);     // attach a pin to the servos and they will start pulsing
  //servoPitch.attach(pitchPin); 
  //servoYaw.attach(yawPin); 
}


// this function just increments a value until it reaches a maximum 
int incPulse(int val, int inc){
   if( val + inc  > 2000 )
      return 1000 ;
   else
       return val + inc;  
}
 int min = 750;
 int max = 2250;
void loop()
{ 
 
    
    int minus30 = map(-40, 40, -40, min, max);
    int zero = map(0, 40, -40, min, max);
    int plus40 = map(40, 40, -40, min, max);

    moveServo(servoRoll,A0,minus30);    
    delay(1000);
    moveServo(servoRoll,A0,zero);
    delay(1000);
    moveServo(servoRoll,A0,plus40);
    delay(1000);
    
/*
 int val;
  
   val = incPulse( servoRoll.read(), 1);
   servoRoll.write(val);

   val =  incPulse( servoPitch.read(), 2);
   servoPitch.write(val);

   if (min > val)
     min = val;

   if (max < val)
     max = val;  


   Serial.print("min ");
   Serial.println(min);
   Serial.print("max ");
   Serial.println(max);

   
   
   val = incPulse(servoYaw.read(), 4);
   servoYaw.write(val);
   
   delay(10);   
   */
   
}

void moveServo(ServoTimer2 servo, int pin, int position)
{   
  servo.attach(pin);  // attaches the servo on pin 9 to the servo object 
  delay(100);
  servo.write(position);
  delay(2000);
  Serial.println(servo.read());
  servo.detach();  // attaches the servo on pin 9 to the servo object 
}
