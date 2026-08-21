#include <Arduino.h>
#include <WiFi.h>
#include "secrets.h"

const char* laptopIP = LAPTOP_ID ;
const uint16_t laptopPort = PORT ;

WiFiClient client;

void setup() {

    Serial.begin(115200);

    Serial.println();
    Serial.println("=== KIM ESP32 TCP TEST ===");

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to Wi-Fi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("Wi-Fi connected");

    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());

    Serial.print("Connecting to laptop: ");
    Serial.print(laptopIP);
    Serial.print(":");
    Serial.println(laptopPort);

    if (client.connect(laptopIP, laptopPort)) {

        Serial.println("TCP connection successful!");

        client.println("Hello from KIM ESP32");

        Serial.println("Message sent.");

        client.stop();

        Serial.println("Connection closed.");

    } else {

        Serial.println("TCP connection FAILED!");
    }
}

void loop() {
}