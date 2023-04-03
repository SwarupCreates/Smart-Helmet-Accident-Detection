#include <SoftwareSerial.h>

SoftwareSerial mySerial(7, 8); // RX, TX pins of the SIM808 module

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect.
  }
  
  mySerial.begin(9600); // Initialize the software serial connection to the SIM808 module

  delay(1000); // Wait for the module to initialize
  mySerial.println("AT+CMGF=1"); // Set the SMS mode to text mode
  delay(1000); // Wait for response
  mySerial.println("AT+CNMI=2,2,0,0,0"); // Set the module to forward received SMS to the Arduino
  delay(1000); // Wait for response    

  mySerial.println("AT+CGNSPWR=1"); // Turn on GPS power
  delay(1000);
  mySerial.println("AT+CGNSSEQ=RMC"); // Turn on GPS rmc mode
  delay(1000);
}

void loop() {
  mySerial.println("AT+CGNSINF"); // Get GPS information
  delay(1000);
  String response = "";
  while (mySerial.available()) {
    response += char(mySerial.read());
  }
  //Serial.println(response);
  String input = response; // Storing the gps response inside a temp string input
  int count = 0; // Initialize the count variable to zero
  int index = 0; // Initialize the index variable to zero
  String lat = ""; // Initialize the ltitude string
  String lng = ""; // Initialize the longitude string

  while (index >= 0) { // Continue until there are no more commas
    index = input.indexOf(",", index+1); // Find the index of the next comma
    count++; // Increment the count variable
    if (count == 3) { // If this is the fourth comma
      lat = input.substring(index+1, input.indexOf(",", index+1)); // Extract the fourth part
    }
    if (count == 4) { // If this is the fifth comma
      lng = input.substring(index+1); // Extract the fifth part
      break; // Exit the loop
    }
  }
  if ((lat!=NULL)&&(lng!=NULL)&&(lat!="0")&&(lng!="0")){
    String googleMapsLink = "http://maps.google.com/maps?q=" + lat + "," + lng; // Construct the Google Maps link
    //Serial.println(googleMapsLink); // Print the Google Maps link in the serial monitor
    delay(1000);
    mySerial.println("ATD9051477045;");
    delay(6000);
    mySerial.println("ATH");
    delay(1000); // Wait for response
    mySerial.println("AT+CMGS=\"+919051477045\""); // Replace with the phone number you want to send the SMS to
    delay(1000); // Wait for response
    String finalMessage = "I've faced an accident at: "+googleMapsLink+" Need help!";
    mySerial.println(finalMessage); // Replace with your own message
    delay(1000); // Wait for response
    mySerial.write(0x1A); // Send the Ctrl+Z character to indicate the end of the message
    delay(1000); // Wait for response
  }
}