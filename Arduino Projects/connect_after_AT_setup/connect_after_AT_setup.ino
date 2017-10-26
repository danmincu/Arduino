#include <SoftwareSerial.h>// import the serial library

#define PIO11 A2             //Arduino pin connected to PI011 of HC-05 (enter AT Mode with HIGH)

SoftwareSerial mySerial(7, 8); // RX, TX




void setup()
{
  
  pinMode(13,OUTPUT);
  pinMode(PIO11, OUTPUT);

  
  digitalWrite(13, LOW);
  Serial.begin(38400);
  Serial.println("Goodnight moon! Start the program while pressing on the button. let go after the long blink every 2 seconds.");
  //mySerial.begin(9600);

mySerial.begin(38400);

delay(1000);
//mySerial.print("AT\r\n");
//Serial.println("AT");
//delay(1000);
//Serial.println("AT+VERSION");
//mySerial.print("AT+VERSION\r\n");

// if (mySerial.available())
//   Serial.write(mySerial.read());

/*
delay(1000);
Serial.println("AT+INIT");
mySerial.print("AT+INIT\r\n");
*/
delay(1000);
Serial.println("AT+PAIR-...");
mySerial.print("AT+PAIR=ac22,0b,5deb16,20\r\n");
//delay(1000);
Serial.println("AT+");
mySerial.print("AT+LINK=000d,18,000001\r\n");

}

void loop() // run over and over
{
if (mySerial.available())
Serial.write(mySerial.read());
if (Serial.available())
mySerial.write(Serial.read());
}
