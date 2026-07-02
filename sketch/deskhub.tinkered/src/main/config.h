#ifndef CONFIG_H
#define CONFIG_H

// Pin definitions
#define PIN_BUZZER        5
#define PIN_ULTRASONIC_ECHO 4
#define PIN_ULTRASONIC_TRIG 3
#define PIN_SOUND_SENSOR  2
#define PIN_LDR           A1
#define PIN_THERMISTOR    A0

// I2C pins (hardware)
#define PIN_SDA           A4
#define PIN_SCL           A5

// OLED I2C address
#define OLED_I2C_ADDRESS  0x3C

// Timing constants (milliseconds)
#define DISPLAY_UPDATE_INTERVAL   500
#define SENSOR_READ_INTERVAL      1000
#define PAGE_CYCLE_INTERVAL       5000
#define ULTRASONIC_TIMEOUT        30000

// Pomodoro constants (milliseconds)
#define POMODORO_WORK_DURATION    (25UL * 60 * 1000)
#define POMODORO_BREAK_DURATION   (5UL * 60 * 1000)

// Thermistor constants (voltage divider with 10k resistor)
#define THERMISTOR_SERIES_RESISTOR 10000.0
#define THERMISTOR_NOMINAL         10000.0
#define THERMISTOR_TEMP_NOMINAL    25.0
#define THERMISTOR_B_COEFFICIENT   3950.0

// LDR constants
#define LDR_SERIES_RESISTOR 10000.0

// Firmware version
#define FIRMWARE_VERSION "1.0.0"

#endif
