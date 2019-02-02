/*
* espbutton_simple.ino
*
* Created by: Ameer Dawood
*
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "Secrets.h"

/*
Secrets.h file should contain data as below:
#define WIFI_SSID "xxxxxxxxxx"
#define WIFI_PASSWORD "xxxxxxxxxx"
#define IFTTT_URL "https://maker.ifttt.com/trigger/button_on/with/key/xxxxxxxxxxxxxxxxxxxxxx"
#define CERT_FINGERPRINT "9a 11 97 ad cd 4f d9 df e4 8f 1b 01 5f 4f 69 d3 87 ea d9 4b"
*/

#define LED_PIN 0
#define BUTTON_PIN 2

void setupWifi(void);
bool postToIfttt(void);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  setupWifi();
}

void loop() {
  if(WiFi.status() == WL_CONNECTED) {
    if(digitalRead(BUTTON_PIN) == LOW){
      Serial.println("Button pressed!");
      digitalWrite(LED_PIN, HIGH);
      delay(200);
      digitalWrite(LED_PIN, LOW);
      if (postToIfttt()) {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("Done!");
      } else {
        Serial.println("IFTTT did not work!");
        digitalWrite(LED_PIN, HIGH);
        delay(200);
        digitalWrite(LED_PIN, LOW);
        delay(200);
        digitalWrite(LED_PIN, HIGH);
        delay(200);
        digitalWrite(LED_PIN, LOW);
        delay(200);
        digitalWrite(LED_PIN, HIGH);
        delay(200);
        digitalWrite(LED_PIN, LOW);
      }
      delay(2000); // button debouncing
      digitalWrite(LED_PIN, LOW);
    }
  } else {
    setupWifi();
  }
}


bool postToIfttt() {
  HTTPClient http;
  uint httpCode;
  http.begin(IFTTT_URL, CERT_FINGERPRINT);
  httpCode = http.GET();
  http.end();
  if (httpCode == 200) {
    Serial.println(httpCode);
    return true;
  } else {
    Serial.println(httpCode);
    return false;
  }
}


void setupWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_PIN, HIGH);
    delay(50);
    digitalWrite(LED_PIN, LOW);
    delay(50);
    digitalWrite(LED_PIN, HIGH);
    delay(50);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(700);
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
}
