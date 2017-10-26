//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <I2C.h>
#include <MMA8453_n0m1.h>


MMA8453_n0m1 accel;

/*******************************************************

This program will test the LCD panel and the buttons
Mark Bramwell, July 2010

********************************************************/

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

SoftwareSerial mySerial(13, 12);

Adafruit_GPS GPS(&mySerial);

#define GPSECHO  true

// this keeps track of whether we're using the interrupt
// off by default!
boolean usingInterrupt = false;
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy


// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

byte smiley[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};

byte speed[8] = {
  B00000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B00000,
};

byte altitude[8] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B01110,
};

byte clearByte[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
};


// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
/* if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  
*/
 // For V1.0 comment the other threshold and use the one below:

 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   



 return btnNONE;  // when all others fail, return this...
}

int gps_power = 11;
bool serialOutput = false;

void setup()
{
  if (serialOutput)
    Serial.begin(9600);
  
  pinMode(gps_power, OUTPUT);
  
  
  accel.setI2CAddr(0x1C); //change your device address if necessary, default is 0x1C
  /*
  threshold [0-127] formula: 0.5g/ 0.063g = 7.9 counts, round to 8 counts ,
  enable X, 
  enable Y,
  enable Z, 
  enable MMA8453Q INT pin 2 (false= pin 1), 
  arduino INT pin number
  */
  accel.motionMode(4,true,false,false,false,2);
  
  
  lcd.createChar(0, smiley);
  lcd.createChar(1, speed);
  lcd.createChar(2, altitude);
  lcd.createChar(3, clearByte);
  lcd.begin(16, 2);              // start the library
// lcd.setCursor(0,0);
// lcd.print("Push the buttons"); // print a simple message
 
 
 
 
 // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  // the nice thing about this code is you can have a timer0 interrupt go off
  // every 1 millisecond, and read data from the GPS for you. that makes the
  // loop code a heck of a lot easier!
  useInterrupt(true);

  delay(1000);
  // Ask for firmware version
  mySerial.println(PMTK_Q_RELEASE);
}




// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;  
    // writing direct to UDR0 is much much faster than Serial.print 
    // but only one character can be written at a time. 
#endif
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}

uint32_t timer = millis();
bool gps_on = false;
bool forced_on = false;
int maxDisplayLevel = 10;
int displayLevel = 2;
int lstate = -1;
float latitude;
float longitude;

uint32_t lastMotion = millis();
bool motion = false;
bool gps_tobe_initialized = true;
bool allowLogging = true;

void loop()
{  

  if (serialOutput)  
  {
    if (gps_on)
      Serial.println("gps_on=true");
    else
      Serial.println("gps_on=false");  
      
    if (forced_on)
      Serial.println("forced_on=true");
    else
      Serial.println("forced_on=false");    
  } 
    
  if (lstate == -1)
    analogWrite(10, displayLevel * 20);
    
  if(gps_on || forced_on)
  {  
        // in case you are not using the interrupt above, you'll
        // need to 'hand query' the GPS, not suggested :(
        if (! usingInterrupt) {
          // read data from the GPS in the 'main loop'
          char c = GPS.read();
          // if you want to debug, this is a good time to do it!
          if (GPSECHO)
            if (c) Serial.print(c);
        }
    
        if (allowLogging && gps_tobe_initialized)
        { 
            gps_tobe_initialized = false;
            GPS.LOCUS_StartLogger();
            lcd.clear();
            lcd.print("logging started");
            delay(500);            
        }    
     
        
        // if a sentence is received, we can check the checksum, parse it...
        if (GPS.newNMEAreceived()) {
          // a tricky thing here is if we print the NMEA sentence, or data
          // we end up not listening and catching other sentences! 
          // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
          //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false
        
          if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
            return;  // we can fail to parse a sentence in which case we should just wait for another
        }        
  }

  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis())  timer = millis();


  accel.update();
  
  if(accel.motion())
  {
    //Serial.println("motion");    
    if (!motion)
    {
     //Serial.println("new motion detected after a quiet period!");    
     
     if (!forced_on)
     {
       lstate = 7;
       lcd.clear();
       
       if (gps_on)
         lcd.print("GPS was on");
       else  
       {
         lcd.print("GPS goes on");     
         gps_on = true;
         digitalWrite(gps_power, HIGH);
         gps_tobe_initialized = true;
       }
       delay(2000);
     }
    }    
     motion = true;
     lastMotion = millis();
     
     lcd.setCursor(15,1);
     lcd.write(byte(0));
     delay(200);
     lcd.setCursor(15,1);
     lcd.write(byte(3));


     //Serial.println("motion!");    
  }

  //detect stop of movement
  if (motion && millis() - lastMotion > 15000)
  {
    if (!GPS.fix || GPS.speed < 3)
    {
     if (!forced_on)
     {
         lstate = 6;
         lcd.clear();
         if (gps_on)
         {
           lcd.print("GPS goes off");
           gps_on = false;
           digitalWrite(gps_power, LOW);
         }
         else  
         {
           lcd.print("GPS was off");
         }
         motion = false;
         delay(2000);
     }
    }
  }
  

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 500) {     

            timer = millis(); // reset the timer
          
            
           lcd.setCursor(0,1);            // move to the begining of the second line
           lcd_key = read_LCD_buttons();  // read the buttons
          
           switch (lcd_key)               // depending on which button was pushed, we perform an action
           {
             case btnRIGHT:
               {
                 //display date time
                 lcd.clear();  
                 lcd.setCursor(0,0);  
                 lcd.print("GMT " + String(GPS.hour) + ":" + String(GPS.minute) + ":" + String(GPS.seconds));
                 lcd.setCursor(0,1);
                 lcd.print("Y/M/D " + String(GPS.year) + "-" + String(GPS.month) + "-" + String(GPS.day));               
                 lstate = 5;
                 delay(3000);
                 break;
               }
             case btnLEFT:
               {
                 allowLogging = !allowLogging;
                 lcd.clear();
                 if (!allowLogging)                 
                   lcd.print("logging OFF");                 
                 else                 
                   lcd.print("logging ON");
                 lstate = 7;  
                 delay(3000);
                 break;
               }
             case btnUP:
               {
               if (displayLevel < maxDisplayLevel)
                  displayLevel++;
               analogWrite(10, displayLevel * 20);               
               if (displayLevel == 1) 
               {                 
                 lcd.display();
                 lcd.clear();
                 lcd.write(byte(0));
                 lcd.setCursor(0,1);
                 if (gps_on)                 
                   lcd.print("GPS is ON");                 
                 else                 
                   lcd.print("GPS is OFF");
                 lstate = 4;  
                 delay(3000);                 
               }
               break;
               }
             case btnDOWN:
               {
               if (displayLevel > 0)
                displayLevel--;
               if (displayLevel == 0) 
               {
                 lcd.noDisplay();
               }
               analogWrite(10, displayLevel * 20);
               
               break;
               
               }
             case btnSELECT:
               {
                 //turn ON/OFF the GPS
                 forced_on = ! forced_on;
                 
                 
                 if (forced_on)
                 {
                   if (lstate != 3)
                   {
                     lcd.clear();
                     lcd.print("GPS ON");
                     digitalWrite(gps_power, HIGH);
                     gps_tobe_initialized = true;
                     lstate = 3;
                   }
                 }
                 else
                 {
                   if (lstate != 2)
                   {
                     lcd.clear();
                     lcd.print("GPS OFF");
                     digitalWrite(gps_power, LOW);
                     lstate = 2;
                   }
                 }
                 delay(3000);                 
                 break;
               }
               case btnNONE:
               {     
                
                if (serialOutput) Serial.println("none");    
                if (gps_on || forced_on)
                {
                      if (serialOutput) Serial.println("forced_on or gps");    
                      if (GPS.fix)
                      {
                        if (serialOutput) Serial.println("gps fix");    
                        if (lstate != 6 || (latitude != conv_coords(GPS.latitude) &&
                        longitude != conv_coords(GPS.longitude)))
                        {
                        
                          latitude = conv_coords(GPS.latitude);
                          longitude = conv_coords(GPS.longitude);

                          lcd.clear();
                          lcd.setCursor(0,0);
                          lcd.print(GPS.lat);                          
                          lcd.print(latitude, 4);
                          lcd.print(GPS.lon);                          
                          lcd.print(longitude, 4);
                          
                          lcd.setCursor(0,1);
                          lcd.write(byte(1));
                          lcd.print(GPS.speed * 1.852, 0);
                          lcd.print("kmph ");
                          lcd.write(byte(2));
                          lcd.print(GPS.altitude,0);                                
                          lcd.print("m");
                          lstate = 6;
                        }
                      }
                      else
                      {
                            if (lstate != 1)
                            {
                              lcd.clear();
                              lcd.print("no GPS fix!");                            
                              lstate = 1;
                            }                           
                      
                      }
                } 
                else
                {
                  if (lstate != 0)
                  {
                   lcd.clear();
                   lcd.print("GPS is off");
                   lcd.setCursor(0,1);
                   lcd.print("SELECT to start");                   
                   lstate = 0;
                  }                  
                }
                break;
               }
           }
  } 
}

float conv_coords(float in_coords)
{
  //Initialize the location.
  float f = in_coords;
  // Get the first two digits by turning f into an integer, then doing an integer divide by 100;
  // firsttowdigits should be 77 at this point.
  int firsttwodigits = ((int)f)/100;                               //This assumes that f < 10000.
  float nexttwodigits = f - (float)(firsttwodigits*100);
  float theFinalAnswer = (float)(firsttwodigits + nexttwodigits/60.0);
  return theFinalAnswer;
}

