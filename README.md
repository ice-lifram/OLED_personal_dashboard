# Ice Lifram's Productivity Desk Dashboard Console "DeskHub"

A modular Arduino Nano project that displays environmental information and dashboard pages on an OLED display. The project is intentionally designed as a **learning-oriented skeleton**, allowing the developer to gradually implement features while maintaining a clean and scalable architecture.

---

# Project Goals

This project aims to:

- Learn modular Arduino programming
- Practice separating code into multiple files
- Understand sensor integration
- Build a simple user interface using an OLED
- Implement page navigation
- Prepare the project for future expansion

Rather than providing a completely finished dashboard, this repository provides the project structure, documentation, and code skeleton needed to build one yourself.

---

# Planned Features

## Environmental Monitoring

- Temperature
- Humidity
- Heat Index
- Comfort Level
- Optional Air Quality
- Optional Light Sensor

---

## Dashboard Pages

Page 1
- Time
- Date

Page 2
- Temperature
- Humidity

Page 3
- Heat Index

Page 4
- System Information

Page 5
- Debug Page

Future pages can easily be added.

---

## Navigation

Button A
- Previous Page

Button B
- Next Page

Long Press
- Reserved

Double Click
- Reserved

---

# Hardware

Current hardware assumes the LAFVIN Starter Kit.

Required:

- Arduino Nano
- SSD1306 OLED Display (I2C)
- DHT11 or DHT22 Sensor
- Push Buttons
- Breadboard
- Jumper Wires

Optional

- MQ135
- LDR
- RTC Module
- Buzzer
- RGB LED

---

# Project Structure

```
nano-productivity-dashboard/

│
├── nano-productivity-dashboard.ino
│
├── config.h
│
├── display.h
├── display.cpp
│
├── sensors.h
├── sensors.cpp
│
├── buttons.h
├── buttons.cpp
│
├── pages.h
├── pages.cpp
│
├── README.md
│
└── GUIDE.md
```

Each file has a single responsibility.

---

# Module Responsibilities

## Main (.ino)

Responsible for:

- setup()
- loop()
- initialization
- calling other modules

Avoid placing application logic here.

---

## display

Responsible for:

- OLED initialization
- Drawing text
- Drawing icons
- Refreshing the display

---

## sensors

Responsible for:

- Reading sensors
- Returning processed values
- Future sensor calibration

---

## buttons

Responsible for:

- Reading buttons
- Debouncing
- Detecting clicks
- Detecting long press

---

## pages

Responsible for:

- Drawing dashboard pages
- Switching pages
- Page-specific logic

---

## config

Stores constants such as

- Screen dimensions
- Pin assignments
- Update intervals
- Feature flags

---

# Recommended Libraries

Adafruit SSD1306

Adafruit GFX

DHT Sensor Library

Bounce2 (optional)

RTClib (future)

---

# Development Philosophy

The project intentionally avoids large monolithic Arduino sketches.

Instead it follows a modular architecture similar to larger software projects.

Benefits include:

- Easier debugging
- Cleaner code
- Easier maintenance
- Easier feature additions

---

# Suggested Development Order

1. OLED Display

2. DHT Sensor

3. Buttons

4. Dashboard Pages

5. System Information

6. Animation

7. Optional Sensors

8. EEPROM Settings

9. Power Saving

10. Data Logging

---

# Future Expansion Ideas

✓ RTC Clock

✓ Weather Station

✓ Air Quality Monitor

✓ Productivity Timer

✓ Pomodoro Timer

✓ Daily Statistics

✓ EEPROM Settings

✓ SD Card Logging

✓ Bluetooth

✓ WiFi Dashboard

✓ IoT Upload

✓ MQTT

✓ Home Assistant Integration

---

# Learning Objectives

By completing this project you should become comfortable with:

- Modular programming
- Header/source separation
- Global state management
- OLED graphics
- Sensor integration
- Event-driven programming
- Simple UI design
- Arduino project organization

---

# License

This project is intended for educational purposes.

Modify, expand, and experiment freely.
