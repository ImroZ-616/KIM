#include "Button.h"

Button::Button(int pin, unsigned long debounceDelay) {
    this->pin = pin;
    this->debounceDelay = debounceDelay;

    buttonState = HIGH;
    lastButtonReading = HIGH;

    lastDebounceTime = 0;

    pressedEvent = false;
    releasedEvent = false;
}

void Button::begin() {
    pinMode(pin, INPUT_PULLUP);
}

void Button::update() {

    int reading = digitalRead(pin);

    pressedEvent = false;
    releasedEvent = false;

    if (reading != lastButtonReading) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {

        if (reading != buttonState) {

            buttonState = reading;

            if (buttonState == LOW) {
                pressedEvent = true;
            } else {
                releasedEvent = true;
            }
        }
    }

    lastButtonReading = reading;
}

bool Button::wasPressed() {
    return pressedEvent;
}

bool Button::wasReleased() {
    return releasedEvent;
}

bool Button::isPressed() {
    return buttonState == LOW;
}