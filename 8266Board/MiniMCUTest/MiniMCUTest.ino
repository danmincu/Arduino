/*
 ESP8266 Blink by Simon Peter
 Blink the blue LED on the ESP-01 module
 This example code is in the public domain
 
 The blue LED on the ESP-01 module is connected to GPIO1 
 (which is also the TXD pin; so we cannot use Serial.print() at the same time)
 
 Note that this sketch uses BUILTIN_LED to find the pin with the internal LED
*/
#define USE_SERIAL Serial
//colors GBR
int pins[] = {12,13,15};
int currentColor = 0;
int sensorPin = A0;
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {  
  
  USE_SERIAL.begin(115200);
  
  attachInterrupt(4, digitalInterrupt, HIGH);
  pinMode(12, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(13, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(15, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
}

// the loop function runs over and over again forever
void loop() {  
  sensorValue = analogRead(sensorPin);
  float delayv = (1024 - sensorValue) / 10.0;
  //USE_SERIAL.print(delayv);
  delay(delayv);
  for (int i = 0; i < 3; i++)  
    digitalWrite(pins[i], LOW);   
  int currentPin = pins[currentColor];  
  delay(delayv);
  digitalWrite(currentPin, HIGH); 
  

  //USE_SERIAL.printf("[Light] %d...\n", sensorValue);
  
  
}


void digitalInterrupt(){
  //digitalWrite(pins[currentColor], LOW); 
  currentColor = (currentColor + 1) % 3;  
}
