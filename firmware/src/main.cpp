#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("--- KIM Core Initialized ---");
}

void loop() {
  Serial.println("KIM is waiting for commands...");
  delay(2000);
}