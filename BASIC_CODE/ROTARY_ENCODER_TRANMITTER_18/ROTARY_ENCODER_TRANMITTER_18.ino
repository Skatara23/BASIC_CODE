#include <Wire.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <espnow.h>

#define encoderCLK_x 5   //D1
#define encoderDT_x 4    //D2
#define encoderSW_x 0    // D3

#define encoderCLK_y 14   //D5
#define encoderDT_y 12    //D6
#define encoderSW_y 13   // D7

void SEND_DATA_TO_API();

const char* SSID = "VMS-Tech";
const char* PASS = "Vms@2023#$";

const char* url = "https://xaapps.com/face_swap/drag/sensordata.php"; // Modified URL

uint8_t broadcastAddress[] = {0x48, 0x55, 0x19, 0xF6, 0x3D, 0xF5};

typedef struct struct_message {
 int x;
 int y;
} struct_message;

struct_message myData;
// SSL fingerprint
const char fingerprint[] = "5D E2 61 86 36 6C 30 74 D3 C9 FD F1 91 D8 E5 54 D6 39 71 2E";

int value_x = 0;
int value_y = 0;

int crntCLK_x;
int prvsCLK_x;
int SWstate_x = 0;

int crntCLK_y;
int prvsCLK_y;
int SWstate_y = 0;

struct Coordinates {
  int x;
  int y;
};

// Declare a pointer to Coordinates struct
Coordinates *c;

WiFiClientSecure client;
HTTPClient http;

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
  pinMode(encoderCLK_x, INPUT);
  pinMode(encoderDT_x, INPUT);
  pinMode(encoderSW_x, INPUT);

  pinMode(encoderCLK_y, INPUT);
  pinMode(encoderDT_y, INPUT);
  pinMode(encoderSW_y, INPUT);

  Serial.begin(115200);
  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  prvsCLK_x = digitalRead(encoderCLK_x);
  prvsCLK_y = digitalRead(encoderCLK_y);

  // Allocate memory for c
  c = new Coordinates;

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Transmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  crntCLK_x = digitalRead(encoderCLK_x);
  SWstate_x = digitalRead(encoderSW_x);

  crntCLK_y = digitalRead(encoderCLK_y);
  SWstate_y = digitalRead(encoderSW_y);

  if (crntCLK_x != prvsCLK_x) {
    if (digitalRead(encoderDT_x) != crntCLK_x) {
      if (SWstate_x == HIGH) {
        value_x -= 128;
      } else {
        value_x -= 1280;
      }
      if (value_x < 0) {
        value_x = 0;
      }
    } else {
      if (SWstate_x == HIGH) {
        value_x += 128;
      } else {
        value_x += 1280;
      }
      if (value_x > 1920) {
        value_x = 1920;
      }
    }
    c->x = value_x;
    // ShowOutput(c);
    SEND_DATA_TO_API();

    myData.x = c->x;
    myData.y = c->y;

    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  }
  prvsCLK_x = crntCLK_x;

  if (crntCLK_y != prvsCLK_y) {
    if (digitalRead(encoderDT_y) != crntCLK_y) {
      if (SWstate_y == HIGH) {
        value_y -= 108;
      } else {
        value_y -= 1080;
      }
      if (value_y < 0) {
        value_y = 0;
      }
    } else {
      if (SWstate_y == HIGH) {
        value_y += 108;
      } else {
        value_y += 1080;
      }
      if (value_y > 1080) {
        value_y = 1080;
      }
    }
    c->y = value_y;
    // ShowOutput(c);
    SEND_DATA_TO_API();
    myData.x = c->x;
    myData.y = c->y;

    //esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  }
  prvsCLK_y = crntCLK_y;

  delay(500); // Adjust delay as needed
}

void SEND_DATA_TO_API() {
  DynamicJsonDocument doc(32);
  doc["x"] = c->x;
  doc["y"] = c->y;

  // Convert JSON to string
  String jsonString;
  serializeJson(doc, jsonString);

  // Set fingerprint
  client.setFingerprint(fingerprint);

  // Modified URL with parameters as JSON format
  String modifiedUrl = String(url) + "?x=" + String(c->x) + "&y=" + String(c->y);

  // Your Domain name with URL path or IP address with path
  http.begin(client, modifiedUrl);

  // Specify content-type header
  http.addHeader("Content-Type", "application/json");

  // Send HTTP POST request
  int httpResponseCode = http.POST(jsonString);

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  Serial.println(http.getString());
  // Free resources
  http.end();

  Serial.print("x:");
  Serial.print(c->x);
  Serial.print(",");
  Serial.print("y:");
  Serial.println(c->y);
}
