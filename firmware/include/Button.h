#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
private:
    int pin;

    int buttonState;
    int lastButtonReading;

    unsigned long lastDebounceTime;
    unsigned long debounceDelay;

public:
    Button(int pin, unsigned long debounceDelay = 50);

    void begin();
    void update();

    bool wasPressed();
    bool wasReleased();
    bool isPressed();

private:
    bool pressedEvent;
    bool releasedEvent;
};

#endif