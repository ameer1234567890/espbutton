/**
* espbutton_lowpower.ino
*
*  Created by: Ameer Dawood
*
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const int LED_PIN = 2;

#define WIFI_SSID "xxxxxxxxxx"
#define WIFI_PASSWORD "xxxxxxxxxx"
#define IFTTT_URL "https://maker.ifttt.com/trigger/button_pressed/with/key/xxxxxxxxxxxxxxxxxxxxxx"
#define CERT_FINGERPRINT "C0 5D 08 5E E1 3E E0 66 F3 79 27 1A CA 1F FC 09 24 11 61 62"

void setupWifi(void);
bool postToIfttt(void);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  setupWifi();

  if (postToIfttt()) {
    digitalWrite(LED_PIN, HIGH);
  }

  delay(2000);
  digitalWrite(LED_PIN, LOW);
  ESP.deepSleep(0); // Enter deep sleep. Sleep forever.
  while(1) {
    __asm__("nop");
  }
}

void loop() {
}

bool postToIfttt() {
  HTTPClient http;
  uint httpCode;
  http.begin(IFTTT_URL, CERT_FINGERPRINT);
  httpCode = http.GET();
  http.end();
  Serial.print(httpCode);
  if (httpCode == 200) {
    return true;
  } else {
    return false;
  }
}

void setupWifi() {
  WiFi.begin("xxxxxxx", "xxxxxxx");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
