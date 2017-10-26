
void setup() 
{
     pinMode(8, OUTPUT);
} 

void loop() 
{ 
     // A total delay of 4 ms = 1/0.004 = 250 Hz… 
     digitalWrite(8, HIGH); 
     delay(2); 
     digitalWrite(8, LOW);
     delay(2);
}
