#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8);
 
char incoming_char=0;
 
void setup()
{
  Serial.begin(19200); // for serial monitor
  SIM900.begin(19200);
  SIM900power();  
  delay(2000);  // give time to log on to network. 
}
 
void SIM900power()
// software equivalent of pressing the GSM shield "power" button
{
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(5000);
}
 
void sendSMS()
{
  SIM900.print("AT+CMGF=1\r");                                                        // AT command to send SMS message
  delay(100);
  SIM900.println("AT + CMGS = \"+16138835817\"");                                     // recipient's mobile number, in international format
  delay(100);
  SIM900.println("This is a test text message. Please confirm you got it...");        // message to send
  delay(100);
  SIM900.println((char)26);                       // End AT command with a ^Z, ASCII code 26
  delay(100); 
  SIM900.println();
  delay(5000);                                     // give module time to send SMS
  SIM900power();                                   // turn off module
}
 
void loop()
{
  Serial.print("sending and receiving an SMS with the GPRS Shield...");
  //sendSMS();
  do {
  
   // Now we simply display any text that the GSM shield sends out on the serial monitor
  if(SIM900.available() >0)
  {
    incoming_char=SIM900.read(); //Get the character from the cellular serial port.
    Serial.print(incoming_char); //Print the incoming character to the terminal.
  }
  else
  delay(1000);
   
  
  } while (1);
  
  
}
