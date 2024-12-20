#include <Wire.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define encoderCLK_x 5   //D1
#define encoderDT_x 4    //D2
#define encoderSW_x 0    // D3

#define encoderCLK_y 14   //D5
#define encoderDT_y 12    //D6
#define encoderSW_y 13   // D7

const char* SSID = "VMS-Tech";
const char* PASS = "Vms@2023#$";

const char* url = "https://xaapps.com/face_swap/apiproject/sensordata.php"; // Modified URL

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

void setup() {
  pinMode(encoderCLK_x, INPUT);
  pinMode(encoderDT_x, INPUT);
  pinMode(encoderSW_x, INPUT);

  pinMode(encoderCLK_y, INPUT);
  pinMode(encoderDT_y, INPUT);
  pinMode(encoderSW_y, INPUT);

  Serial.begin(9600);
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
  }
  prvsCLK_y = crntCLK_y;

  // Serialize Coordinates struct to JSON
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

  // Show on Serial Monitor
  Serial.print("x:");
  Serial.print(c->x);
  Serial.print(",");
  Serial.print("y:");
  Serial.println(c->y);

  delay(1000); // Adjust delay as needed
}


