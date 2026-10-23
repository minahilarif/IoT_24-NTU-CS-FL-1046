#include <Arduino.h>

// ---------- Pins ----------
const int LED1_PIN = 18;
const int LED2_PIN = 19;

// ---------- PWM settings ----------
// Channels 0 and 1 share one LEDC timer (same frequency), so LED2 uses channel 2
// to get its own timer and a different frequency.
const int LED1_CH = 0;
const int LED2_CH = 2;
const int LED1_FREQ = 1000;   // Hz
const int LED2_FREQ = 5000;   // Hz
const int PWM_RES = 10;       // bits -> duty range 0..1023
const int MAX_DUTY = (1 << PWM_RES) - 1;

// ---------- Fade state (these values are also used for the OLED in Task 2) ----------
int duty1 = 0;
int duty2 = MAX_DUTY;
int dir1 = 1;
int dir2 = -1;
const int STEP1 = 4;
const int STEP2 = 6;
const unsigned long FADE_INTERVAL = 20;   // ms between fade steps
unsigned long lastFade = 0;
unsigned long lastPrint = 0;

int dutyToPercent(int duty) {
  return (duty * 100 + MAX_DUTY / 2) / MAX_DUTY;
}

void pwmBegin() {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcAttach(LED1_PIN, LED1_FREQ, PWM_RES);
  ledcAttach(LED2_PIN, LED2_FREQ, PWM_RES);
#else
  ledcSetup(LED1_CH, LED1_FREQ, PWM_RES);
  ledcAttachPin(LED1_PIN, LED1_CH);
  ledcSetup(LED2_CH, LED2_FREQ, PWM_RES);
  ledcAttachPin(LED2_PIN, LED2_CH);
#endif
}

void pwmWrite(int led, int duty) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcWrite(led == 1 ? LED1_PIN : LED2_PIN, duty);
#else
  ledcWrite(led == 1 ? LED1_CH : LED2_CH, duty);
#endif
}

// Moves duty up or down and reverses direction at 0 and MAX_DUTY
int nextDuty(int duty, int &dir, int step) {
  duty += dir * step;
  if (duty >= MAX_DUTY) { duty = MAX_DUTY; dir = -1; }
  else if (duty <= 0)   { duty = 0;        dir = 1;  }
  return duty;
}

void setup() {
  Serial.begin(115200);
  pwmBegin();
  pwmWrite(1, duty1);
  pwmWrite(2, duty2);
  Serial.println("Dual LED PWM fade started");
}

void loop() {
  unsigned long now = millis();

  if (now - lastFade >= FADE_INTERVAL) {
    lastFade = now;
    duty1 = nextDuty(duty1, dir1, STEP1);
    duty2 = nextDuty(duty2, dir2, STEP2);
    pwmWrite(1, duty1);
    pwmWrite(2, duty2);
  }

  if (now - lastPrint >= 500) {
    lastPrint = now;
    Serial.printf("LED1: %d%% F:%dHz | LED2: %d%% F:%dHz\n",
                  dutyToPercent(duty1), LED1_FREQ,
                  dutyToPercent(duty2), LED2_FREQ);
  }
}