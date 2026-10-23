# Embedded-IoT-Homework-2 - Dual-LED PWM Control with OLED Monitoring

Course: Embedded IoT Systems
Name: Minahil Arif | Roll No: 24-NTU-CS-FL-1046

## Wiring
| Component | ESP32 pin |
|---|---|
| LED1 (via resistor) | GPIO 18 |
| LED2 (via resistor) | GPIO 19 |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |
| OLED VCC / GND | 3V3 / GND |

Each LED has its own current-limiting resistor. LED cathodes go to GND.

## PWM frequencies
| LED | LEDC channel | Frequency | Resolution |
|---|---|---|---|
| LED1 | 0 | 1000 Hz | 10 bit (0-1023) |
| LED2 | 2 | 5000 Hz | 10 bit (0-1023) |

LEDC timer limitation: on the ESP32, channels 0 and 1 share one LEDC timer, channels 2 and 3
share the next one, and so on. Channels sharing a timer must use the same frequency and resolution.
LED2 uses channel 2 so that it gets a different timer and can run at a different frequency.
With Arduino-ESP32 core 3.x the timer is chosen automatically.

## Duty-cycle control
Both LEDs fade from 0 to 1023 and back. LED1 changes by 4 per step and LED2 by 6 per step,
every 20 ms, in opposite directions. Duty is converted to percent as duty x 100 / 1023.
Duty cycle is the fraction of time the pin is ON. A higher duty gives a higher average
voltage and a brighter-looking LED. Frequency is the number of ON/OFF cycles per second;
both frequencies used here are fast enough that no flicker is visible.

## OLED output
The OLED shows the title PWM MONITOR and one line per LED with its current duty percent
and configured frequency. The OLED text uses the same variables as the PWM output
(duty1, duty2, LED1_FREQ, LED2_FREQ), so the displayed values match the real values.

## Test results
### Task 1 (two LEDs, no OLED)
- Wokwi: both LEDs fade at different frequencies
- Hardware: Leds were fading with a frequency difference
- Evidence: screenshots/task1_wokwi_circuit_1046.png, task1_wokwi_running_1046.png,
  task1_hardware_greenfade_1046.jpeg, task1_hardware_yellowfade_1046.jpeg

### Task 2 (with OLED)
- Wokwi: OLED values were chnaging
- Hardware: Frequency change in percentage
- Evidence: screenshots/wokwi_circuit_1046.png, wokwi_running_oled_1046.png,
  hardware_circuit_output_oled_1046.jpg


## Links
- Wokwi project: https://wokwi.com/projects/478666625110583297