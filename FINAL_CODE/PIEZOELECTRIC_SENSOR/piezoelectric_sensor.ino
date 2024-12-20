#include <ESP8266WiFi.h>

// Define the pin connected to the sensor
const int sensorOutput = A0; // Analog pin connected to the sensor
const int ledOutput = 16;    // Pin connected to LED

const int THRESHOLD = 20;

void setup() {
  pinMode(ledOutput, OUTPUT);   // Declare the LED pin as output
  Serial.begin(9600);
}

void loop() {
  int value = analogRead(sensorOutput);  // Read analog voltage from sensor
  Serial.print(value);
  delay(100);
  Serial.print("\n");
  if (value >= THRESHOLD) {
    digitalWrite(ledOutput, HIGH);
    delay(500); // To make the LED visible
  } else {
    digitalWrite(ledOutput, LOW);
  }
  
}
