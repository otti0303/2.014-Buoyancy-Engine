#include <Arduino.h>
#include <Wire.h>
#include "config.h"

#define LED_PIN = 2 // change accordingly

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("ESP32-S3 Initialized!");
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // LED
  digitalWrite(LED_PIN, HIGH);
}