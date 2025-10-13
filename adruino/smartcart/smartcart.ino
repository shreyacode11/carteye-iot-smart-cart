#include <WiFi.h>
#include <HTTPClient.h>
#include "HX711.h"

// ===== CONFIG =====
const char* ssid     = "your wifi name";
const char* password = "your password";
const char* FIREBASE_URL = "https://smartcart6-default-rtdb.firebaseio.com/sensors/latest.json";

// ===================

HX711 scale;
const int LOADCELL_DOUT_PIN = 19;
const int LOADCELL_SCK_PIN  = 18;

// Ultrasonic sensor pins (optional)
const int TRIG_PIN = 5;
const int ECHO_PIN = 4;

// HX711 calibration factor
float calibration_factor = -7050.0;

// Track last sent values
float lastSentWeight = 0;
float lastSentHeight = 0;
unsigned long lastSendMillis = 0;
const unsigned long SEND_INTERVAL = 1200; // ms

void setup() {
  Serial.begin(115200);
  delay(100);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  // HX711 setup
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);
  scale.tare(); // zero

  // Ultrasonic setup
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("Setup done.");
}

float readWeightGrams() {
  float val = scale.get_units(10); // average 10 readings
  if (val < 0.0) val = 0.0;
  Serial.printf("Weight: %.2f g\n", val);
  return val;
}

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return -1.0;
  float distance = (duration * 0.0343) / 2.0;
  Serial.printf("Distance: %.2f cm\n", distance);
  return distance;
}

void sendToFirebase(float weight, float height) {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  http.begin(FIREBASE_URL); // Must include .json
  http.addHeader("Content-Type", "application/json");

  String payload = "{";
  payload += "\"weight\":" + String(weight, 2) + ",";
  payload += "\"height\":" + String(height, 2) + ",";
  payload += "\"timestamp\":" + String(millis());
  payload += "}";

  int httpCode = http.PUT(payload);
  if (httpCode > 0) {
    Serial.printf("Sent: %s  resp=%d\n", payload.c_str(), httpCode);
  } else {
    Serial.printf("HTTP error: %d\n", httpCode);
  }
  http.end();
}

void loop() {
  float w = readWeightGrams();
  float h = readDistanceCm();

  // If weight is very small, treat as item removed
  if (w < 5.0) {
    w = 0;
    h = 0;
  }

  // Only send if values changed significantly or interval passed
  if ((fabs(w - lastSentWeight) > 0.5) || (fabs(h - lastSentHeight) > 0.5) ||
      (millis() - lastSendMillis > SEND_INTERVAL)) {
    lastSentWeight = w;
    lastSentHeight = h;
    lastSendMillis = millis();
    sendToFirebase(w, h);
  }

  delay(300);
}

