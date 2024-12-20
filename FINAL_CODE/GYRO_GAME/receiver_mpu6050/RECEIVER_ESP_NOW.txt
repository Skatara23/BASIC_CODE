#include <Wire.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <MPU6050.h>

#define LED_PIN 12
#define MPU_ADDR 0x68 // MPU6050 I2C address

// Structure example to receive data
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

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  
  // Handle received data
  //Serial.print("Command: ");
  //Serial.println(myData.command);
  //Serial.print("Accelerometer X: ");
  Serial.print(myData.accelerometerX);
  // //Serial.print("\tAccelerometer Y: ");
   Serial.print(" , ");
   Serial.print(myData.accelerometerY);
   Serial.print(" , ");
  //Serial.print("\tAccelerometer Z: ");
  Serial.println(myData.accelerometerZ);
  //Serial.print(" , ");
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  delay(100);

  // Initialize the I2C bus
  Wire.begin();

  // Initialize the MPU6050 sensor
  mpu.initialize();

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register receive callback function
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // No need for any code here
}
