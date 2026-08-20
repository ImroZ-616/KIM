#include <Arduino.h>
#include <WiFi.h>
#include "secrets.h"

const char* ssid     = WIFI_SSID;
const char* password = WIFI_PASSWORD;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n=================================");
  Serial.println("   KIM Wi-Fi Diagnostic Tool    ");
  Serial.println("=================================");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to: ");
  Serial.println(ssid);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[SUCCESS] Wi-Fi connected");
    Serial.print("ESP32 IP address : ");
    Serial.println(WiFi.localIP());
    Serial.print("Gateway          : ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("Subnet Mask      : ");
    Serial.println(WiFi.subnetMask());
    Serial.print("RSSI (Signal)    : ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("\n[FAILED] Could not connect to Wi-Fi.");
    Serial.println("Please check SSID, password, or router 2.4 GHz band.");
  }
  Serial.println("=================================\n");
}

void loop() {
  // Keep alive
  delay(10000);
}