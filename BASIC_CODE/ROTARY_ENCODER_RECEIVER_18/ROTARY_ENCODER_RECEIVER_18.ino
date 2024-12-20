#include <espnow.h>

// Structure to hold received data
struct EncoderData {
  int x;
  int y;
};

EncoderData receivedData;

// Callback function to handle received data
void onDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));

  // Print received data
  Serial.print("Received Data - X: ");
  Serial.print(receivedData.x);
  Serial.print(", Y: ");
  Serial.println(receivedData.y);
}

void setup() {
  Serial.begin(115200);

  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register ESP-NOW receive callback
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onDataRecv);
}

void loop() {
  // No need for code here, all handling is done in the receive callback
}
