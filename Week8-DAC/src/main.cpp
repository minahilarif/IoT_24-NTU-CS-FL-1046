#include <Arduino.h>

#define DAC_PIN 25       // DAC1: steady analog voltage
#define PWM_PIN 27       // PWM: repeated HIGH/LOW pulses
#define PWM_CHANNEL 0
#define PWM_FREQ 5000    // Hz
#define PWM_RES 8        // bits -> duty 0..255
#define VALUE 128        // same number used by both outputs

void setup()
{
    Serial.begin(115200);

    // PWM setup on GPIO27
#if ESP_ARDUINO_VERSION_MAJOR >= 3
    ledcAttach(PWM_PIN, PWM_FREQ, PWM_RES);
    ledcWrite(PWM_PIN, VALUE);
#else
    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RES);
    ledcAttachPin(PWM_PIN, PWM_CHANNEL);
    ledcWrite(PWM_CHANNEL, VALUE);
#endif

    // DAC output on GPIO25
    dacWrite(DAC_PIN, VALUE);

    Serial.println("Simultaneous DAC (GPIO25) and PWM (GPIO27), both with value 128");
}

void loop()
{
}