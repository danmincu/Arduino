#include <ServoTimer2.h>
#include <VirtualWire.h>

ServoTimer2 servoT;    // declare variables for up to eight servos
ServoTimer2 servoW;

int min = 750;
int max = 2250;

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
    
  /*  moveServo(servoT,A0,0);
    delay(2000);
    moveServo(servoT,A0,1000);
    delay(2000);
    moveServo(servoT,A0,2000);
    delay(2000);
    */
    
  
  
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
       
       //controlling temperature
       
       if (message[0] == 'T')
       {         
          byte *tempOnly = &message[1];
          tempOnly[messageLength-1] = 0;          
          String st = String((char*)tempOnly);
          int value = st.toInt();
          Serial.println(messageLength);
          Serial.println(st);
          Serial.println(value);
          
          int tPosition = map(value, 40, -40, min, max);
          moveServo(servoT, A0, tPosition);
         
/*         
          char sT[messageLength - 1];
          for (int i = 1; i < messageLength; i++)
          {
            sT[i-1] = message[i];
          }    
          sT[messageLength - 1] = 0;
          Serial.println("controlling temperature");
          String st = String(sT);
          int value = st.toInt();
          Serial.println(value);
          */
       }
       
       if (message[0] == 'W')
       {         
          byte *tempOnly = &message[1];
          tempOnly[messageLength-1] = 0;          
          String st = String((char*)tempOnly);
          int value = st.toInt();
          Serial.println(messageLength);
          Serial.println(st);
          Serial.println(value);
          
          int tPosition = map(value, 10, 0, min, max);
          moveServo(servoW, A1, tPosition);
        }     
       
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



void moveServo(ServoTimer2 servo, int pin, int position)
{   
  servo.attach(pin);  // attaches the servo on pin 9 to the servo object 
  delay(100);
  servo.write(position);
  delay(2000);
  Serial.println(servo.read());
  servo.detach();  // attaches the servo on pin 9 to the servo object 
}



