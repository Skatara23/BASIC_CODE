#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Wire.h>
#include <MPU6050.h>

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x48, 0x55, 0x19, 0xF6, 0x3D, 0xF5};

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
const int buttonPin = 5; // Assuming the button is connected to GPIO pin D1

// Variable to store the state of the button
int buttonState = 0;

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
}

void loop() {
  // Read the state of the button
  buttonState = digitalRead(buttonPin);

  // Read accelerometer data
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Print accelerometer data
  //Serial.print("Accelerometer X: ");
  
  Serial.print(ax / 16384.0); // Convert raw data to g
  Serial.print(" , ");
  Serial.print(ay / 16384.0); // Convert raw data to g
  Serial.print(" , ");
  Serial.println(az / 16384.0); // Convert raw data to g

  // Set the command based on the button state
  if (buttonState == HIGH) {
    myData.command = '1'; // Turn on the LED
    //Serial.println('1');
  } else {
    myData.command = '0'; // Turn off the LED
    //Serial.println('0');
  }

  // Set accelerometer data
  myData.accelerometerX = ax / 16384.0; // Convert raw data to g
  myData.accelerometerY = ay / 16384.0; // Convert raw data to g
  myData.accelerometerZ = az / 16384.0; // Convert raw data to g

  // Send message via ESP-NOW
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  delay(100); // Debounce delay
}
