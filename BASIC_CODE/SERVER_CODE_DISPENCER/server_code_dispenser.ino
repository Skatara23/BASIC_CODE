#include <ESP8266WiFi.h>
#include <SocketIOClient.h>
 
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* serverAddress = "YOUR_SERVER_IP_OR_HOSTNAME";
const int serverPort = 3000;
 
SocketIOClient client;
 
void setup() {
  Serial.begin(115200);
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
 
  // Connect to server
  client.begin(serverAddress, serverPort);
  client.on("connect", []() {
    Serial.println("Connected to server");
  });
  client.on("disconnect", []() {
    Serial.println("Disconnected from server");
  });
 
  // Listen for chat messages from server
  client.on("tab_message", [](const char* payload, size_t length) {
    Serial.println(payload);
  });
}
 
void loop() {
  // Maintain connection to the server
  client.loop();
 
  // Example code to send a chat message
  // You can modify this to read from Serial or any other input
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');
    client.emit("tab_message", message.c_str());
  }
 
  delay(100);
}