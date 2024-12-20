// Include required libraries
#include <ESP8266WiFi.h>

// Define relay pin
const int relayPin = 5;

// WiFi credentials
const char* ssid = "VMS-Tech";
const char* password = "Vms@2023#$";

// Initialize WiFi client
WiFiClient client;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize relay pin as an output
  pinMode(relayPin, OUTPUT);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi.");
}

void loop() {
  // Check if data is available to read from serial
  if (Serial.available() > 0) {
    // Read the incoming byte
    char receivedChar = Serial.read();

    // Toggle relay based on received character
    if (receivedChar == '1') {
      digitalWrite(relayPin, HIGH); // Turn relay on
      Serial.println("Relay is ON.");
    } else if (receivedChar == '0') {
      digitalWrite(relayPin, LOW); // Turn relay off
      Serial.println("Relay is OFF.");
    }
  }
}
