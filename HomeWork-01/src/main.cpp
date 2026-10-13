#include <Arduino.h>

#define BUTTON_PIN 18

const unsigned long DEBOUNCE_MS = 50;

int pressCount = 0;
int lastReading = HIGH;
int stableState = HIGH;
unsigned long lastChangeTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastReading) {
    lastChangeTime = millis();
    lastReading = reading;
  }

  if ((millis() - lastChangeTime) >= DEBOUNCE_MS && reading != stableState) {
    stableState = reading;
    if (stableState == LOW) {
      pressCount++;
      Serial.print("Accepted press #");
      Serial.println(pressCount);
    }
  }
}