#include <Arduino.h>
#include "Button.h"

const int BUTTON_PIN = 4;

Button button1(BUTTON_PIN);
Button button2(5);
Button button3(18);
Button button4(19);

void setup() {

    Serial.begin(115200);

    button1.begin();
    button2.begin();
    button3.begin();
    button4.begin();

    Serial.println("KIM butttons is ready.....");
}

void loop() {

    button1.update();
    button2.update();
    button3.update();
    button4.update();

    if (button1.wasPressed()) {
        Serial.println("BUTTON 1 PRESSED");
    }

    if (button1.wasReleased()) {
        Serial.println("BUTTON 1 RELEASED");
    }
    if (button2.wasPressed()) {
        Serial.println("BUTTON 2 PRESSED");
    }

    if (button2.wasReleased()) {
        Serial.println("BUTTON 2 RELEASED");
    }
    if (button3.wasPressed()) {
        Serial.println("BUTTON 3 PRESSED");
    }

    if (button3.wasReleased()) {
        Serial.println("BUTTON 3 RELEASED");
    }
    if (button4.wasPressed()) {
        Serial.println("BUTTON 4 PRESSED");
    }

    if (button4.wasReleased()) {
        Serial.println("BUTTON 4 RELEASED");
    }
    
}