#include <SoftwareSerial.h>// import the serial library

#define PIO11 A2             //Arduino pin connected to PI011 of HC-05 (enter AT Mode with HIGH)

SoftwareSerial mySerial(7, 8); // RX, TX


void enterATMode()
{
  Serial.print("Enter AT mode");
  
 mySerial.flush();
 delay(500);
 digitalWrite(PIO11, HIGH);
 //resetBT();
 delay(500);
 mySerial.begin(38400);//HC-05 AT mode baud rate is 38400
 
}

//----------------------------------------------------------//
//--------Enter HC-05 bluetooth moduel command mode---------//
//-------------set HC-05 mode pin to LOW--------------------//
void enterComMode()
{
  Serial.print("Enter Command mode");
  
 mySerial.flush();
 delay(500);
 digitalWrite(PIO11, LOW);
 //resetBT();
 delay(500);
 mySerial.begin(38400); //default communication baud rate of HC-05 is 38400
}


void setup()
{
  
  pinMode(13,OUTPUT);
  pinMode(PIO11, OUTPUT);

  
  digitalWrite(13, LOW);
  Serial.begin(38400);
  Serial.println("Goodnight moon! Start the program while pressing on the button. let go after the long blink every 2 seconds.");
  //mySerial.begin(9600);

delay(3000);
enterATMode();
//mySerial.begin(38400);

//mySerial.begin(38400);

delay(1000);
mySerial.print("AT\r\n");
Serial.println("AT");
delay(1000);
Serial.println("AT+VERSION");
mySerial.print("AT+VERSION\r\n");

 if (mySerial.available())
   Serial.write(mySerial.read());


delay(1000);
//mySerial.print("AT+PIN1342"); // Set pin to 1342
//delay(1000);
//mySerial.print("AT+NAME=HC-05\r\n"); // Set the name to FresaBT
//delay(1000);
//mySerial.print("AT+BAUD8"); // Set baudrate to 115200
//mySerial.print("AT+BAUD4\r\n"); // Set baudrate to 9600

Serial.println("AT+ROLE=1");
mySerial.print("AT+ROLE=1\r\n"); // Set the name to FresaBT

delay(1000);
Serial.println("AT+CMODE=0");
mySerial.print("AT+CMODE=0\r\n");

delay(1000);
Serial.println("AT+BIND=....");
mySerial.print("AT+BIND=ac22,0b,5deb16\r\n");

delay(1000);
Serial.println("AT+INIT");
mySerial.print("AT+INIT\r\n");

delay(1000);
Serial.println("AT+PAIR-...");
mySerial.print("AT+PAIR=ac22,0b,5deb16,20\r\n");
//delay(1000);
Serial.println("AT+");
mySerial.print("AT+LINK=000d,18,000001\r\n");

 //enterComMode();
}

void loop() // run over and over
{
if (mySerial.available())
Serial.write(mySerial.read());
if (Serial.available())
mySerial.write(Serial.read());
}
