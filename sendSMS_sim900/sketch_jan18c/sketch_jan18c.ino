#include <SoftwareSerial.h>

SoftwareSerial GPRS(7, 8);         //7 = TX, 8 = RX
unsigned char buffer[64]; //port
int count=0;     
int i = 1;                         //if i = 0, send SMS.
void setup(){
  //delay(10000);
  GPRS.begin(19200);               // the GPRS baud rate   
  Serial.begin(19200);             // the Serial port of Arduino baud rate.
  Serial.print("I'm ready");
  Serial.print("Hello?"); 
}

void loop(){
  if (GPRS.available()){         // if date is comming from softwareserial port ==> data is comming from gprs shield

    while(GPRS.available()){          // reading data into char array 
      buffer[count++]=GPRS.read();     // writing data into array
      if(count == 64)break;
    }
  Serial.write(buffer,count);            // if no data transmission ends, write buffer to hardware serial port
  clearBufferArray();              // call clearBufferArray function to clear the storaged data from the array
  count = 0;                       // set counter of while loop to zero


}
if (Serial.available())            // if data is available on hardwareserial port ==> data is comming from PC or notebook
  GPRS.write(Serial.read());       // write it to the GPRS shield

  if(i == 0){
    GPRS.print("AT+CMGF=1\r"); //mandando SMS em modo texto
    Serial.println("AT+CMGF=1\r"); //mandando SMS em modo texto
    delay(1000);
    //GPRS.print("AT+CMGS=\"+554988063979\"\r"); // numero que vamos mandar o SMS
    Serial.println("AT+CMGS=\"+554988063979\"\r"); // numero que vamos mandar o SMS
    delay(1000);
    //GPRS.print("Test\r"); // corpo da msg
    Serial.println("Test\r"); // corpo da msg
    delay(1000);
    //GPRS.write(0x1A); //equivalente a mandar Ctrl+Z(finaliza corpo do SMS)
    delay(1000);
    Serial.println("SMS sent successfully");
    i++;
  }   

}
void clearBufferArray(){              // function to clear buffer array
  for (int i=0; i<count;i++){
    buffer[i]=NULL;                  // clear all index of array with command NULL
  }
}
