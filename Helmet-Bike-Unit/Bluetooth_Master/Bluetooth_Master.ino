#include <SoftwareSerial.h>
#include <math.h>
#include <Wire.h>

//master button
//#define button 2
//int value = 0;

//ultrasonic declarations
const int trigPin = 9;
const int echoPin = 10;
long duration;
int distanceCm, distanceInch;
const int proximityThreshold = 5;

//mq3 gas declarations
#define sensorAnalog A1
#define sensorDigital 3
const int alcoholThreshold = 20;
const int sampleCollectible = 6;
int initialMean;

void setup() {

  //alcohol digital pin
  pinMode(sensorDigital, INPUT);

  //ultrasonic pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);

  //To calculate ambient air index
  int sum = 0;  // Variable to store the sum of the readings
  int analog;  // Variable to store each reading

  // Take the first sampleCollectible readings and calculate the initial mean
  bool digital = digitalRead(sensorDigital);
  for (int i = 0; i < sampleCollectible; i++) {
    analog = analogRead(sensorAnalog);
    sum += analog;
    delay(200);
  }
  initialMean = sum / sampleCollectible;

}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.034 / 2;

  bool digital = digitalRead(sensorDigital);
  
  int analog;  // Variable to store each reading
  Serial.println(initialMean);
  analog = analogRead(sensorAnalog);
  int thresholdModulus  = (analog - initialMean);

  //can drive condition
  if(distanceCm < proximityThreshold && thresholdModulus < alcoholThreshold)
  {
    Serial.println("A");
    value = 1;
    delay(200);
  }
  else if(distanceCm >= proximityThreshold || thresholdModulus >= alcoholThreshold)
  {
    Serial.println("a");
    value = 0;
    delay(200);
  }
  delay(200);
}
