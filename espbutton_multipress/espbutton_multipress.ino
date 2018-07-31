/*
* espbutton_multipress.ino
*
* Created by: Ameer Dawood
*
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define LED_PIN 0
#define BUTTON_PIN 2

#define WIFI_SSID "xxxxxxxxxx"
#define WIFI_PASSWORD "xxxxxxxxxx"
#define IFTTT_ON_URL "https://maker.ifttt.com/trigger/button_on/with/key/xxxxxxxxxxxxxxxxxxxxxx"
#define IFTTT_OFF_URL "https://maker.ifttt.com/trigger/button_off/with/key/xxxxxxxxxxxxxxxxxxxxxx"
#define CERT_FINGERPRINT "C0 5D 08 5E E1 3E E0 66 F3 79 27 1A CA 1F FC 09 24 11 61 62"

byte switchState = 1;
byte lastState = 1;
word shortDuration = 1000;
word longDuration = 2000;
unsigned long switchTime = 0;
bool buttonDownLongNotified = false;

void setupWifi(void);
bool postToIfttt(String);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  setupWifi();
}

void loop() {
  if(WiFi.status() == WL_CONNECTED) {
    if (switchState == 0) {
      if ((millis() - switchTime) >= longDuration) {
        if (buttonDownLongNotified == false) {
          buttonDownLongNotified = true;
          digitalWrite(LED_PIN, HIGH);
          Serial.println("Button is down for long time");
        }
      }
    }
    switchState = digitalRead(BUTTON_PIN);
    if (!switchState && lastState) {  // just pressed
      lastState = switchState;
      switchTime = millis(); // start timer
    }
    if (switchState && !lastState) {   // just released
      if (((millis() - switchTime) <= shortDuration) && ((millis() - switchTime) <= longDuration)) { // short press
        Serial.println("Button pressed: short");
        digitalWrite(LED_PIN, HIGH);
        delay(shortDuration);
        digitalWrite(LED_PIN, LOW);
        if (postToIfttt(IFTTT_ON_URL)) {
          Serial.println("Done!");
          digitalWrite(LED_PIN, HIGH);
          digitalWrite(LED_PIN, LOW);
        } else {
          Serial.println("IFTTT did not work!");
          for (int i=0; i <= 3; i++){
            digitalWrite(LED_PIN, HIGH);
            delay(200);
            digitalWrite(LED_PIN, LOW);
            delay(200);
          }
        }
        lastState = switchState;
      } else if ((millis() - switchTime) > longDuration) { // long press
        Serial.println("Button pressed: long");
        buttonDownLongNotified = false;
        digitalWrite(LED_PIN, HIGH);
        delay(longDuration);
        digitalWrite(LED_PIN, LOW);
        if (postToIfttt(IFTTT_OFF_URL)) {
          Serial.println("Done!");
          digitalWrite(LED_PIN, HIGH);
          digitalWrite(LED_PIN, LOW);
        } else {
          Serial.println("IFTTT did not work!");
          for (int i=0; i <= 3; i++){
            digitalWrite(LED_PIN, HIGH);
            delay(200);
            digitalWrite(LED_PIN, LOW);
            delay(200);
          }
        }
        lastState = switchState;
      }
    }
  } else {
    setupWifi();
  }
}

bool postToIfttt(String url) {
  HTTPClient http;
  uint httpCode;
  http.begin(url, CERT_FINGERPRINT);
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
