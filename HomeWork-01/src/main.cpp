#include <Arduino.h>

#define BUTTON_PIN 18

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Button pressed (raw, no debounce)");
  }
}