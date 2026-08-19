const int BUTTON_PIN = 4;

int buttonState = HIGH;
int lastButtonReading = HIGH;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.println("KIM Button Ready...");
}

void loop() {

  int reading = digitalRead(BUTTON_PIN);

  // Button signal changed
  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
  }

  // Signal has remained stable
  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonState) {
      buttonState = reading;

      // Button pressed
      if (buttonState == LOW) {
        Serial.println("BUTTON PRESSED");
      }

      // Button released
      else {
        Serial.println("BUTTON RELEASED");
      }
    }
  }

  lastButtonReading = reading;
}