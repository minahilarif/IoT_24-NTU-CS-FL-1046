#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BUTTON_PIN 18
#define SDA_PIN 21
#define SCL_PIN 22
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C

const unsigned long DEBOUNCE_MS = 50;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int pressCount = 0;
int lastReading = HIGH;          // raw value from last loop
int stableState = HIGH;          // debounced value
unsigned long lastChangeTime = 0;

void drawScreen() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("ESP32 Button Counter");
  display.setCursor(0, 20);
  display.print("Button: ");
  display.println(stableState == LOW ? "PRESSED" : "RELEASED");
  display.setTextSize(2);
  display.setCursor(0, 40);
  display.print("Count: ");
  display.print(pressCount);
  display.display();             // push buffer to screen
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("OLED not found");
    while (true) delay(1000);
  }
  drawScreen();
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);

  // any change restarts the stability timer, so bounce is ignored
  if (reading != lastReading) {
    lastChangeTime = millis();
    lastReading = reading;
  }

  // accept the new state only after it stayed stable for DEBOUNCE_MS
  if ((millis() - lastChangeTime) >= DEBOUNCE_MS && reading != stableState) {
    stableState = reading;
    if (stableState == LOW) {      // valid press
      pressCount++;
      Serial.print("Accepted press #");
      Serial.println(pressCount);
    }
    drawScreen();
  }
}