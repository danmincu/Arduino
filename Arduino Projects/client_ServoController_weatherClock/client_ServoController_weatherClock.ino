
#include <VirtualWire.h>

#include <Servo.h> 
 
Servo myservoT;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
Servo myservoW;  // create servo object to control a servo 


void setup()
{
  Serial.begin(9600);
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_set_rx_pin(12);
    vw_setup(4000);  // Bits per sec
    pinMode(13, OUTPUT);

    vw_rx_start();       // Start the receiver PLL running
}
    void loop()
{
    
    byte message[VW_MAX_MESSAGE_LEN]; // a buffer to store the incoming messages
    byte messageLength = VW_MAX_MESSAGE_LEN; // the size of the message
    
    if (vw_get_message(message, &messageLength)) // Non-blocking
    { 
       Serial.print("Received: ");
       for (int i = 0; i < messageLength; i++)
       {
         Serial.write(message[i]);
       }
         Serial.println();
     }

  /*  
  
      uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

  
  if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      if(buf[0]=='T'){
        
        Serial.println(buflen);
        
        for (int i = 1; i < buflen; i++)
        {
          Serial.print(buf[i]);
        }
          
   digitalWrite(13,1);
      }  
   if(buf[0]=='W'){
  digitalWrite(13,0);
    }

}*/
}

void moveServo(int position)
{   
  myservoT.attach(A0);  // attaches the servo on pin 9 to the servo object 
  delay(50);
  myservoT.write(position);
  delay(200);
  myservoT.detach();  // attaches the servo on pin 9 to the servo object 
}



