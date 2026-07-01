# Nano Productivity Dashboard
## Developer Guide

Version 1.0

---

# Introduction

Welcome!

This project is intentionally designed as a **learning-first Arduino project**.

Instead of providing a massive `.ino` file containing every feature, the project is divided into independent modules that each have a single responsibility.

The goal is to help you learn software architecture while building a useful dashboard.

---

# Project Philosophy

This project follows five principles:

1. Small functions
2. One responsibility per module
3. Centralized configuration
4. Expandability
5. Readability over cleverness

If you continue following these principles, your future Arduino projects will become much easier to maintain.

---

# Folder Structure

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

---

# File Responsibilities

## nano-productivity-dashboard.ino

Think of this as the **conductor**.

It should **not** contain application logic.

Instead, it coordinates the modules.

Good:

- setup()
- loop()
- Scheduling

Avoid:

- Drawing pages
- Reading sensors directly
- Complex calculations

---

## config.h

Contains every value that users are expected to customize.

Examples:

- Pins
- Delays
- Display size
- Feature flags
- Calibration values

Whenever possible, avoid hardcoding values anywhere else.

---

## display.cpp

Responsible only for graphics.

It should know:

- How to draw text
- How to draw shapes
- How to update the OLED

It should **not** know:

- Sensor logic
- Button logic
- Page navigation

---

## sensors.cpp

The only module that communicates with sensors.

Every sensor reading should pass through this module.

Never call sensor libraries directly from page rendering functions.

---

## buttons.cpp

The only module allowed to read hardware buttons.

If another module needs to know whether a button is pressed, it should ask:

```
nextPagePressed()
```

instead of calling `digitalRead()`.

---

## pages.cpp

Decides **what** appears on the display.

It does not know how to communicate with the OLED.

Instead, it uses drawing functions supplied by the display module.

---

# Program Flow

```
loop()

│

├── updateButtons()

├── handleNavigation()

├── updateSensors()

└── refreshDashboard()
```

Inside `refreshDashboard()`:

```
drawCurrentPage()

↓

renderCurrentPage()

↓

renderEnvironmentPage()

↓

drawLeftText()

↓

OLED
```

---

# How to Add a New Dashboard Page

Suppose you want a Battery page.

### Step 1

Add it to the enum.

```
PAGE_BATTERY
```

---

### Step 2

Declare it.

```
void renderBatteryPage();
```

---

### Step 3

Implement it.

```
void renderBatteryPage()
{
    ...
}
```

---

### Step 4

Update the switch statement.

```
case PAGE_BATTERY:
    renderBatteryPage();
    break;
```

Finished.

---

# How to Add a New Sensor

Suppose you install an MQ135.

### Step 1

Add variables.

```
float airQuality;
```

---

### Step 2

Implement

```
readMQ135()
```

---

### Step 3

Call it.

```
readAllSensors();
```

---

### Step 4

Display it.

Create a page or extend an existing one.

No other module should require changes.

---

# Coding Style

Prefer:

```
if(condition)
{
}
```

instead of

```
if ( condition ) {
}
```

Use descriptive names.

Good:

```
updateSensors()

refreshDashboard()
```

Poor:

```
update()

work()

thing()
```

---

# Common Beginner Mistakes

## Everything in loop()

Avoid writing hundreds of lines inside `loop()`.

---

## Copy-paste code

If you repeat code three times, consider writing a function.

---

## Hardcoded values

Avoid:

```
display.drawRect(0,0,128,64);
```

Prefer:

```
SCREEN_WIDTH

SCREEN_HEIGHT
```

---

## Global variables everywhere

Keep variables as local as possible.

---

## Blocking delays

Avoid:

```
delay(5000);
```

Prefer:

```
millis()
```

when implementing periodic tasks.

---

# Suggested Learning Exercises

## Beginner

- Add a new dashboard page.
- Display firmware version.
- Add a loading screen.

---

## Intermediate

- Add automatic page rotation.
- Add a stopwatch.
- Add a Pomodoro timer.
- Add settings.
- Add EEPROM storage.

---

## Advanced

- Battery monitoring.
- RTC clock.
- SD card logging.
- Weather station.
- ESP8266 Wi-Fi.
- MQTT.
- Home Assistant.
- OTA updates.
- Web dashboard.

---

# Suggested Git Commits

```
Initial project structure

Display module

Sensor module

Button module

Page manager

Configuration

Documentation

Animations

Optional sensors

Settings menu
```

Small, focused commits make it easier to understand your project's evolution.

---

# Testing Checklist

## Display

- OLED initializes
- Splash screen appears
- Text alignment looks correct

---

## Sensors

- Temperature updates
- Humidity updates
- Heat index is reasonable
- Sensor failures are handled gracefully

---

## Buttons

- Previous page
- Next page
- No repeated presses
- Debouncing works

---

## Navigation

- Wraps correctly
- No invalid page numbers

---

# Future Ideas

This skeleton can grow into many different projects.

Examples:

- Weather Station
- Study Dashboard
- Smart Desk
- Environmental Logger
- Plant Monitor
- Air Quality Monitor
- Home Automation Display
- IoT Dashboard
- Industrial Status Panel

The architecture is intentionally flexible enough to support these ideas with minimal restructuring.

---

# Final Advice

Don't try to implement every feature at once.

Work in small, testable steps:

1. Make one change.
2. Compile.
3. Upload.
4. Test.
5. Commit.

Repeat.

Small progress compounds into a reliable project.

---

# Closing Thoughts

The goal of this repository isn't just to build a dashboard.

It's to help you become comfortable with:

- Modular programming
- Clean architecture
- Embedded systems
- Debugging
- Incremental development

Once you understand this project, you'll have a solid foundation for much larger Arduino, ESP32, and embedded C++ applications.

Happy building!
