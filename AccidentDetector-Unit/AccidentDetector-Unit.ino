#include<LiquidCrystal_I2C.h>
#include <AltSoftSerial.h>
#include <SoftwareSerial.h>
#include <math.h>
#include <Wire.h>

//must add i2c lcd address use i2c-scanner.ino file
LiquidCrystal_I2C lcd(0x27, 16, 2);

//--------------------------------------------------------------
#define BUZZER 12
#define BUTTON 11
#define relay 3
//--------------------------------------------------------------
#define xPin A1
#define yPin A2
#define zPin A3
//--------------------------------------------------------------

byte updateflag;

int xaxis = 0, yaxis = 0, zaxis = 0;
int deltx = 0, delty = 0, deltz = 0;
int vibration = 2, devibrate = 75;
int magnitude = 0;
int sensitivity = 160;
double angle;
boolean impact_detected = false;
//Used to run impact routine every 2mS.
unsigned long time1;
unsigned long impact_time;
unsigned long alert_delay = 30000; //30 seconds
//--------------------------------------------------------------




/*****************************************************************************************
 * setup() function
 *****************************************************************************************/
void setup()
{
  //--------------------------------------------------------------
  //Serial.println("Arduino serial initialize");
  Serial.begin(9600);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(relay, OUTPUT);
  //--------------------------------------------------------------
  //initialize lcd screen
  lcd.begin();
  // turn on the backlight
  lcd.backlight();
  lcd.clear();

  time1 = micros(); 
  xaxis = analogRead(xPin);
  yaxis = analogRead(yPin);
  zaxis = analogRead(zPin);
}

void loop()
{
  //--------------------------------------------------------------
  //call impact routine every 2mS
  if (micros() - time1 > 1999){ 
  Impact();
  if(updateflag > 0) 
  {
    updateflag=0;
    Serial.println("Impact detected!!");
    Serial.print("Magnitude:"); Serial.println(magnitude);

    digitalWrite(BUZZER, HIGH);
    impact_detected = true;
    impact_time = millis();
    
    lcd.clear();
    lcd.setCursor(0,0); //col=0 row=0
    lcd.print("Crash Detected");
    lcd.setCursor(0,1); //col=0 row=1
    lcd.print("Magnitude:"+String(magnitude));
  }
  //--------------------------------------------------------------
  if(impact_detected == true)
  {
    if(millis() - impact_time >= alert_delay) {
      digitalWrite(BUZZER, LOW);
      delay(1000);
      impact_detected = false;
      impact_time = 0;
    }
    else{
    digitalWrite(relay, HIGH);
    }
  }
  
  if(digitalRead(BUTTON) == LOW){
    delay(200);
    digitalWrite(BUZZER, LOW);
    digitalWrite(relay, LOW);    
    impact_detected = false;
    impact_time = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Be Careful!");
    delay(3000);
    lcd.clear();
  }
  }
}





/*****************************************************************************************
 * Impact() function
 *****************************************************************************************/
void Impact()
{
  //--------------------------------------------------------------
  time1 = micros(); // resets time value
  //--------------------------------------------------------------
  int oldx = xaxis; //store previous axis readings for comparison
  int oldy = yaxis;
  int oldz = zaxis;

  xaxis = analogRead(xPin);
  yaxis = analogRead(yPin);
  zaxis = analogRead(zPin);
  
  //--------------------------------------------------------------
  //loop counter prevents false triggering. Vibration resets if there is an impact. Don't detect new changes until that "time" has passed.
  vibration--; 
  //Serial.print("Vibration = "); Serial.println(vibration);
  if(vibration < 0) vibration = 0;                                
  //Serial.println("Vibration Reset!");
  
  if(vibration > 0) return;
  //--------------------------------------------------------------
  deltx = xaxis - oldx;                                           
  delty = yaxis - oldy;
  deltz = zaxis - oldz;
  
  //Magnitude to calculate force of impact.
  magnitude = sqrt(sq(deltx) + sq(delty) + sq(deltz));
  //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
  if (magnitude >= sensitivity) //impact detected
  {
    updateflag=1;
    // reset anti-vibration counter
    vibration = devibrate;
  }
  //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
  else
  {
    //if (magnitude > 15)
      //Serial.println(magnitude);
    //reset magnitude of impact to 0
    magnitude=0;
  }
  //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
}


