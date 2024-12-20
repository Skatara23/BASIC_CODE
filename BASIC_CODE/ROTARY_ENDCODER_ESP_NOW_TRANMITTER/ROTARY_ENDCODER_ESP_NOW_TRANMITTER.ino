#include <Wire.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

#define encoderCLK_x 5   // D1
#define encoderDT_x 4    // D2
#define encoderSW_x 0    // D3

#define encoderCLK_y 14   // D5
#define encoderDT_y 12    // D6
#define encoderSW_y 13   // D7

uint8_t broadcastAddress[] = {0x48, 0x55, 0x19, 0xf6, 0x3d, 0xf5};

struct EncoderData {
  int x;
  int y;
};

EncoderData encoderData;

void setup() {
  pinMode(encoderCLK_x, INPUT);
  pinMode(encoderDT_x, INPUT);
  pinMode(encoderSW_x, INPUT_PULLUP);

  pinMode(encoderCLK_y, INPUT);
  pinMode(encoderDT_y, INPUT);
  pinMode(encoderSW_y, INPUT_PULLUP);

  Serial.begin(115200);
  
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register peer (receiver)
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  // Read encoder data
  encoderData.x = readEncoder(encoderCLK_x, encoderDT_x);
  encoderData.y = readEncoder(encoderCLK_y, encoderDT_y);

  // Send data via ESP-NOW
  esp_now_send(broadcastAddress, (uint8_t*)&encoderData, sizeof(encoderData));

  delay(100); // Adjust delay as needed
}

int readEncoder(int clkPin, int dtPin) {
  static int counter = 0;
  static int clkStateLast = LOW;
  int clkState = digitalRead(clkPin);
  int dtState = digitalRead(dtPin);

  if (clkState != clkStateLast) {
    if (dtState != clkState) {
      counter--;
    } else {
      counter++;
    }
  }
  clkStateLast = clkState;
  
  return counter;
}
