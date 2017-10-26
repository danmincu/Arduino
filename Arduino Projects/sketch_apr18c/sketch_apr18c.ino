//Make sure the dip switches are turned ON, and none of your shields are using pins A0,A1,A2,A3 or D4

#include <Shieldbot.h>	//includes the Shieldbot Library
#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


Shieldbot shieldbot = Shieldbot();	//decares a Shieldbot object
int S1,S2,S3,S4,S5;	//values to store state of sensors

void setup(){
  Serial.begin(115200);//Begin serial comm for debugging
  shieldbot.setMaxSpeed(50,50);//255 is max, if one motor is faster than another, adjust values 
}

void loop(){
  
//  delay(400);
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  Serial.print("Ping: ");
  int distance = uS / US_ROUNDTRIP_CM;
  Serial.print(distance); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");
  
  //Read all the sensors 
  S1 = shieldbot.readS1();
  S2 = shieldbot.readS2();
  S3 = shieldbot.readS3();
  S4 = shieldbot.readS4();
  S5 = shieldbot.readS5();

  //Print the status of each sensor to the Serial console
  Serial.print("S5: ");
  Serial.print(S5);
  Serial.print(" S4: ");
  Serial.print(S4);
  Serial.print(" S3: ");
  Serial.print(S3);
  Serial.print(" S2: ");
  Serial.print(S2);
  Serial.print(" S1: ");
  Serial.print(S1);
  Serial.println();
 
  //Note about IR sensors	
  //if a sensor sees HIGH, it means that it just sees a reflective surface background(ex. whie)
  //if a sensor sees LOW, it means that it sees a non-reflective surface or empty space (ex. black tape line, or empty space off ledge)
 
  if(S1 == HIGH && S5 == HIGH){	//if the two outer IR line sensors see background, go forward
   if (distance < 20 && distance > 0)
    {    
        Serial.println("avoid");
        shieldbot.backward();
        delay(500);
        shieldbot.drive(-128,127);
        delay(500);
    }
    else
      {
        shieldbot.drive(95,127);
        //shieldbot.forward();
        Serial.println("Forward");
      }

  }else if(S1 == LOW && S5 == LOW){	//if either of the two outer IR line sensors see empty space (like edge of a table) stop moving
     shieldbot.stop();
     Serial.println("Stop");
     delay(100);
  }
   else if((S1 == LOW) || (S2 == LOW)){	//if the two most right IR line sensors see black tape, turn right
    shieldbot.drive(127,-128);// to turn right, left motor goes forward and right motor backward
	Serial.println("Right");
    delay(100);
  }else if((S5 == LOW) || (S4 == LOW)){	//if either of the two most left IR line sensors see black , turn left
    shieldbot.drive(-128,127);// to turn right, left motor goes backward and right motor forward
	Serial.println("Left");
    delay(100);
  }else	//otherwise just go forward
  {
    if (distance < 10)
    {    
      shieldbot.forward();
    }
    else
      {
        shieldbot.backward();
        delay(300);
        shieldbot.drive(-128,127);
        delay(300);
      }
  }
}
