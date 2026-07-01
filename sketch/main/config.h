/*
==========================================================
 config.h
----------------------------------------------------------
 Project Configuration

 PURPOSE

    Central location for all user-editable settings.

    Hardware changes should normally only require
    modifications to this file.

==========================================================
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <DHT.h>



//==========================================================
// PROJECT INFORMATION
//==========================================================

#define PROJECT_NAME        "Nano Productivity Dashboard"

#define PROJECT_VERSION     "1.0.0"

#define AUTHOR_NAME         "Ice Lifram"



//==========================================================
// SERIAL
//==========================================================

constexpr unsigned long SERIAL_BAUDRATE = 115200;



//==========================================================
// DISPLAY
//==========================================================

constexpr uint8_t SCREEN_WIDTH = 128;

constexpr uint8_t SCREEN_HEIGHT = 64;

constexpr uint8_t OLED_RESET_PIN = -1;

constexpr uint8_t OLED_I2C_ADDRESS = 0x3C;



//==========================================================
// SENSOR CONFIGURATION
//==========================================================

constexpr uint8_t DHT_PIN = 2;

// Change to DHT22 if needed.
constexpr uint8_t DHT_TYPE = DHT11;



//==========================================================
// BUTTONS
//==========================================================

constexpr uint8_t PREVIOUS_BUTTON_PIN = 3;

constexpr uint8_t NEXT_BUTTON_PIN = 4;



//==========================================================
// TIMING
//==========================================================

// Startup delay after reset.
constexpr unsigned long STARTUP_DELAY = 500;

// Splash screen duration.
constexpr unsigned long SPLASH_DURATION = 1500;

// Sensor polling interval.
constexpr unsigned long SENSOR_INTERVAL = 2000;

// Display refresh interval.
constexpr unsigned long DISPLAY_INTERVAL = 100;



//==========================================================
// BUTTON TIMING
//==========================================================

constexpr unsigned long BUTTON_DEBOUNCE_MS = 30;

constexpr unsigned long LONG_PRESS_MS = 800;

constexpr unsigned long DOUBLE_CLICK_MS = 300;



//==========================================================
// FEATURE FLAGS
//==========================================================

constexpr bool ENABLE_SERIAL_DEBUG = true;

constexpr bool ENABLE_SPLASH_SCREEN = true;

constexpr bool ENABLE_ANIMATIONS = false;

constexpr bool ENABLE_PROGRESS_BAR = true;

constexpr bool ENABLE_DEBUG_PAGE = true;



//==========================================================
// SENSOR CALIBRATION
//==========================================================

// Adjust if your sensor reads consistently high/low.

constexpr float TEMPERATURE_OFFSET = 0.0f;

constexpr float HUMIDITY_OFFSET = 0.0f;



//==========================================================
// FUTURE EXPANSION
//==========================================================

// Uncomment when new hardware is installed.

// constexpr uint8_t MQ135_PIN = A0;

// constexpr uint8_t LDR_PIN = A1;

// constexpr uint8_t BUZZER_PIN = 5;

// constexpr uint8_t RGB_RED_PIN = 6;

// constexpr uint8_t RGB_GREEN_PIN = 7;

// constexpr uint8_t RGB_BLUE_PIN = 8;



//==========================================================
// UI SETTINGS
//==========================================================

constexpr uint8_t DEFAULT_TEXT_SIZE = 1;

constexpr uint8_t TITLE_TEXT_SIZE = 2;

constexpr uint8_t PAGE_MARGIN = 2;



//==========================================================
// PAGE SETTINGS
//==========================================================

constexpr bool WRAP_PAGE_NAVIGATION = true;

constexpr bool AUTO_PAGE_ROTATION = false;

constexpr unsigned long AUTO_PAGE_INTERVAL = 5000;



//==========================================================
// DEBUG
//==========================================================

constexpr bool PRINT_SENSOR_REPORT = false;

constexpr bool PRINT_BUTTON_REPORT = false;

constexpr bool PRINT_PAGE_REPORT = false;

constexpr bool PRINT_DISPLAY_REPORT = false;



#endif
