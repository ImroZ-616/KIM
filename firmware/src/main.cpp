#include <Arduino.h>

#include "Button.h"
#include "Display.h"

const int BUTTON_PIN = 4;

Button button1(BUTTON_PIN);
Button button2(5);
Button button3(18);
Button button4(19);

Display display;

void setup() {

    Serial.begin(115200);

    button1.begin();
    button2.begin();
    button3.begin();
    button4.begin();

    if (!display.begin()) {
        Serial.println("Display initialization failed!");
    } else {
        Serial.println("Display initialized!");
    }

    Serial.println("KIM buttons are ready...");
}

void loop() {

    button1.update();
    button2.update();
    button3.update();
    button4.update();

    if (button1.wasPressed()) {
        Serial.println("BUTTON 1 PRESSED");
        display.showText("BUTTON 1");
    }

    if (button1.wasReleased()) {
        Serial.println("BUTTON 1 RELEASED");
    }

    if (button2.wasPressed()) {
        Serial.println("BUTTON 2 PRESSED");
        display.showText("BUTTON 2");
    }

    if (button2.wasReleased()) {
        Serial.println("BUTTON 2 RELEASED");
    }

    if (button3.wasPressed()) {
        Serial.println("BUTTON 3 PRESSED");
        display.showText("BUTTON 3");
    }

    if (button3.wasReleased()) {
        Serial.println("BUTTON 3 RELEASED");
    }

    if (button4.wasPressed()) {
        Serial.println("BUTTON 4 PRESSED");
        display.showText("BUTTON 4");
    }

    if (button4.wasReleased()) {
        Serial.println("BUTTON 4 RELEASED");
    }
}