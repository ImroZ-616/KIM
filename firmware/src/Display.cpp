#include "Display.h"

Display::Display()
    : oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {
}

bool Display::begin() {
    Wire.begin(21, 22);
    if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        return false;
    }

    oled.clearDisplay();
    oled.setTextColor(SSD1306_WHITE);
    oled.setTextSize(2);
    oled.setCursor(0, 0);

    oled.println("KIM");

    oled.display();

    return true;
}

void Display::clear() {

    oled.clearDisplay();
    oled.display();
}

void Display::showText(const char* text) {

    oled.clearDisplay();

    oled.setTextColor(SSD1306_WHITE);
    oled.setTextSize(2);
    oled.setCursor(0, 0);

    oled.println(text);

    oled.display();
}