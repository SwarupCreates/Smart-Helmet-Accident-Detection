#include <SoftwareSerial.h>
#include <math.h>
#include<Wire.h>

#define led 2
#define relayPin 3

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(relayPin, OUTPUT);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  if(Serial.available())
  {
    char val = Serial.read();
    if(val == 'A')
    {
      digitalWrite(led, HIGH);
      digitalWrite(relayPin, HIGH);
    }
    else if(val == 'a')
    {
      digitalWrite(led, LOW);
      digitalWrite(relayPin, LOW);
    }
  }
}