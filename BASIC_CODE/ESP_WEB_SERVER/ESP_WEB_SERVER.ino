#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Wire.h>
#include <MPU6050.h>
#include <ESP8266HTTPClient.h>

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xBC, 0xFF, 0x4D, 0x39, 0x7E, 0x7D};

// Structure example to send data
typedef struct struct_message {
  char command; // Command to control the LED ('1' to turn on, '0' to turn off)
  float accelerometerX; // Accelerometer data X-axis
  float accelerometerY; // Accelerometer data Y-axis
  float accelerometerZ; // Accelerometer data Z-axis
} struct_message;

// Create a struct_message called myData
struct_message myData;

// MPU6050 object
MPU6050 mpu;

// Pin connected to the button
const int buttonPin = 13; // Assuming the button is connected to GPIO pin D1

// Variable to store the state of the button
int buttonState = 0;
int previousButtonState = LOW; // Initialize previous button state

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  //Serial.print("Last Packet Send Status: ");
  // if (sendStatus == 0){
  //   Serial.println("Delivery success");
  // }
  // else{
  //   Serial.println("Delivery fail");
  // }
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  // Init Wire library
  Wire.begin();

  // Init MPU6050
  mpu.initialize();

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Connect to Wi-Fi
  WiFi.begin("VMS-Tech", "Vms@2023#$");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Transmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  // Set button pin as input
  pinMode(buttonPin, INPUT);

  pinMode(12, OUTPUT);
}

void loop() {
  // Read the state of the button
  buttonState = digitalRead(buttonPin);

  // Only send data if the button state changes from LOW to HIGH (pressed)
  if (buttonState == HIGH && previousButtonState == LOW) {
    // Read accelerometer data
    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);

    // Print accelerometer data
    Serial.print("Accelerometer X: ");
    Serial.print(ax / 16384.0); // Convert raw data to g
    Serial.print(" , ");
    Serial.print(ay / 16384.0); // Convert raw data to g
    Serial.print(" , ");
    Serial.print(az / 16384.0); // Convert raw data to g
   
    // Set the command based on the button state
    myData.command = '1'; // Turn on the LED

    // Set accelerometer data
    myData.accelerometerX = ax / 16384.0; // Convert raw data to g
    myData.accelerometerY = ay / 16384.0; // Convert raw data to g
    myData.accelerometerZ = az / 16384.0; // Convert raw data to g
    if (myData.accelerometerX >= 0.04 && myData.accelerometerX <= 0.10) {
      digitalWrite(12, LOW); // Turn on the LED
    } else {
      digitalWrite(12, HIGH); // Turn off the LED
    }

    // Send message via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    // Prepare JSON payload for HTTP POST request
    String payload = "{\"x-axis\":" + String(myData.accelerometerX) + ",\"y-axis\":" + String(myData.accelerometerY) + ",\"z-axis\":" + String(myData.accelerometerZ) + "}";

    
    // Create a WiFiClient object
    WiFiClient client;
    
    // Send HTTP POST request to the API endpoint
    HTTPClient http;
    http.begin(client, "https://xaapps.com/face_swap//apiproject/sensordata.php"); // Update API usage
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(payload);
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String response = http.getString();
      Serial.println(response);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }

  previousButtonState = buttonState; // Update previous button state
  delay(50); // Debounce delay
}
