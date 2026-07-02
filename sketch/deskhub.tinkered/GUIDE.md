# DeskHub Firmware Architecture Guide

## Table of Contents
1. [Overview](#overview)
2. [Project Structure](#project-structure)
3. [Architecture Philosophy](#architecture-philosophy)
4. [Module Descriptions](#module-descriptions)
5. [How Pages Work](#how-pages-work)
6. [How Sensors Work](#how-sensors-work)
7. [How Timing Works](#how-timing-works)
8. [Adding New Pages](#adding-new-pages)
9. [Adding New Sensors](#adding-new-sensors)
10. [Adding New Hardware](#adding-new-hardware)
11. [Extending the Pomodoro System](#extending-the-pomodoro-system)
12. [Memory Management](#memory-management)
13. [Debugging](#debugging)
14. [Future Expansion Ideas](#future-expansion-ideas)

---

## Overview

**DeskHub** is a modular, maintainable Arduino Nano firmware project designed as a desktop dashboard. Unlike typical Arduino sketches that cram everything into `setup()` and `loop()`, DeskHub follows embedded systems best practices:

- **Modular design**: Separate managers for display, pages, sensors, timers
- **Non-blocking execution**: Uses `millis()` instead of `delay()`
- **Clean separation of concerns**: Each module has a single responsibility
- **Easy extensibility**: Add new pages, sensors, or features without restructuring
- **Standalone operation**: Works without Serial Monitor (Serial only for debugging)

---

## Project Structure

```
DeskHub/
├── platformio.ini          # PlatformIO configuration
├── GUIDE.md               # This file
├── wiring/
│   └── wiring.json        # Circuit wiring (DSL format)
└── src/
    ├── main.cpp           # Entry point (setup/loop)
    ├── config.h           # Pin definitions, constants, configuration
    ├── Utility.h/cpp      # Helper functions (uptime, RAM, formatting)
    ├── DisplayManager.h/cpp    # OLED display abstraction
    ├── SensorManager.h/cpp     # Sensor reading and processing
    ├── PageManager.h/cpp       # Page switching and rendering
    ├── PomodoroManager.h/cpp   # Pomodoro timer logic
    └── QuoteManager.h/cpp      # Quote storage and retrieval
```

### File Responsibilities

| File | Purpose |
|------|---------|
| `main.cpp` | Initialization and main loop coordination |
| `config.h` | All pin mappings, timing constants, thresholds |
| `Utility.h/cpp` | System utilities (uptime, free RAM, string formatting) |
| `DisplayManager` | Low-level OLED control, text rendering, layout |
| `SensorManager` | Read and process all sensors (thermistor, LDR, ultrasonic, sound) |
| `PageManager` | High-level page logic, page switching, rendering coordinator |
| `PomodoroManager` | Pomodoro timer state machine (work/break/pause) |
| `QuoteManager` | Store and retrieve motivational quotes |

---

## Architecture Philosophy

### 1. **Separation of Concerns**
Each manager handles ONE responsibility:
- `DisplayManager` knows HOW to draw on the OLED, but not WHAT to draw
- `PageManager` knows WHAT to display, but delegates drawing to DisplayManager
- `SensorManager` reads sensors, but doesn't decide what to do with the data
- `PomodoroManager` handles timer logic, but doesn't know about the display

### 2. **Non-Blocking Execution**
All timing uses `millis()` checks:
```cpp
// BAD (blocking)
delay(1000);

// GOOD (non-blocking)
if (millis() - lastUpdate >= 1000) {
    lastUpdate = millis();
    // do work
}
```

This allows:
- Multiple timers running simultaneously
- Responsive sensor reading
- Smooth page transitions
- Future button handling without lag

### 3. **Manager Pattern**
Each manager is a class with:
- `begin()` or `init()` for initialization
- `update()` for periodic tasks (called every loop)
- Public methods for external interaction
- Private state and helper methods

### 4. **Configuration Centralization**
All hardware-specific values live in `config.h`:
- Pin numbers
- Timing intervals
- Sensor thresholds
- Display dimensions

**Why?** Change hardware (e.g., move buzzer from D5 to D6) by editing ONE line in `config.h`, not hunting through multiple files.

---

## Module Descriptions

### DisplayManager
**Purpose**: Abstract the OLED hardware from the rest of the code.

**Key Methods**:
- `begin()`: Initialize the display
- `clear()`: Clear the screen
- `setCursor(x, y)`: Position text cursor
- `print(text)`: Draw text at cursor
- `setTextSize(size)`: Change text size
- `drawLine()`, `drawRect()`, `fillRect()`: Shapes
- `display()`: Push buffer to screen

**Why separate?**: If you switch from SSD1306 to a different display (e.g., LCD, TFT), you only rewrite DisplayManager. The rest of the code stays the same.

**Usage Example**:
```cpp
displayMgr.clear();
displayMgr.setCursor(0, 0);
displayMgr.setTextSize(2);
displayMgr.print("Hello");
displayMgr.display();
```

---

### SensorManager
**Purpose**: Read all sensors and provide processed values.

**Key Methods**:
- `begin()`: Initialize sensor pins
- `update()`: Read sensors periodically (non-blocking)
- `getTemperature()`: Returns temperature in °C
- `getLightLevel()`: Returns 0-100% light level
- `isSoundDetected()`: Returns true if sound detected
- `getDistance()`: Returns ultrasonic distance in cm
- `isPresenceDetected()`: Returns true if object within threshold

**Sensor Processing**:
- **Thermistor**: Reads ADC, applies Steinhart-Hart equation
- **LDR**: Reads ADC, maps to 0-100% scale
- **Sound Sensor**: Digital read (HIGH = sound detected)
- **Ultrasonic**: Triggers measurement, reads echo pulse width

**Why separate?**: 
- Sensor logic is complex (calibration, filtering, conversion)
- Easy to add new sensors without touching page code
- Can mock sensors for testing

**Adding Sensor Filtering**:
```cpp
// In SensorManager::update()
float rawTemp = readThermistor();
temperature = (temperature * 0.9) + (rawTemp * 0.1); // Exponential moving average
```

---

### PageManager
**Purpose**: Coordinate page switching and rendering.

**Key Methods**:
- `begin()`: Initialize page system
- `update()`: Handle page switching and rendering
- `nextPage()`, `prevPage()`: Manual navigation (for future buttons)
- `getCurrentPageName()`: Get active page name

**Page Structure**:
Each page is a function:
```cpp
void renderHomePage() {
    displayMgr.clear();
    // Draw page content
    displayMgr.display();
}
```

**Page Array**:
```cpp
const PageInfo pages[] = {
    {"Home", renderHomePage},
    {"Pomodoro", renderPomodoroPage},
    // ...
};
```

**Auto-Cycling**:
```cpp
if (millis() - lastPageSwitch >= PAGE_SWITCH_INTERVAL) {
    currentPage = (currentPage + 1) % pageCount;
    lastPageSwitch = millis();
}
```

**Why separate?**: 
- Easy to add/remove pages
- Page order defined in one place
- Can disable auto-cycling for manual navigation
- Future: add page-specific input handling

---

### PomodoroManager
**Purpose**: Manage Pomodoro timer state machine.

**States**:
1. **WORK**: Work session (25 minutes default)
2. **BREAK**: Break session (5 minutes default)
3. **PAUSED**: Timer paused

**Key Methods**:
- `begin()`: Initialize timer
- `update()`: Decrement timer, handle state transitions
- `pause()`, `resume()`: Control timer
- `getRemainingMinutes()`, `getRemainingSeconds()`: Get time left
- `getState()`: Get current state (WORK/BREAK/PAUSED)
- `getSessionNumber()`: Get current work session count

**State Machine**:
```
WORK (25 min) → BREAK (5 min) → WORK (25 min) → ...
     ↓                ↓
   PAUSED ←──────────┘
```

**Auto-Transition**:
When timer reaches 0:
- WORK → BREAK (increment session count)
- BREAK → WORK
- Optional: Buzzer beep on transition

**Why separate?**: 
- Complex state logic isolated
- Easy to adjust durations
- Can add long breaks (every 4 sessions)
- Future: Save state to EEPROM

---

### QuoteManager
**Purpose**: Store and retrieve motivational quotes.

**Key Methods**:
- `getRandomQuote()`: Returns a random quote
- `getQuoteCount()`: Returns total number of quotes

**Storage**:
```cpp
const char* quotes[] = {
    "Stay focused!",
    "One task at a time.",
    // ...
};
```

**Why separate?**: 
- Easy to add/edit quotes
- Could load from SD card in future
- Could add categories or filtering

---

### Utility
**Purpose**: System-level helper functions.

**Key Functions**:
- `getUptime()`: Returns formatted uptime string (HH:MM:SS)
- `getFreeRAM()`: Returns available RAM in bytes
- `formatTime(minutes, seconds)`: Format time as MM:SS

**Why separate?**: 
- Reusable across modules
- System-level functions in one place
- Easy to add new utilities

---

## How Pages Work

### Page Lifecycle

1. **Registration**: Pages are defined in `PageManager` constructor
2. **Switching**: `PageManager::update()` checks timer and switches pages
3. **Rendering**: Current page's render function is called
4. **Display**: Render function uses `DisplayManager` to draw content

### Page Rendering Pattern

Every page follows this pattern:
```cpp
void renderMyPage() {
    displayMgr.clear();              // 1. Clear screen
    
    // 2. Draw title
    displayMgr.setCursor(0, 0);
    displayMgr.setTextSize(1);
    displayMgr.print("Page Title");
    
    // 3. Draw content
    displayMgr.setCursor(0, 16);
    displayMgr.print("Content here");
    
    displayMgr.display();            // 4. Push to screen
}
```

### Page Layout Guidelines

**OLED Dimensions**: 128x64 pixels

**Recommended Layout**:
```
┌────────────────────┐
│ Title (y=0)        │  ← Text size 1 (8px high)
├────────────────────┤
│                    │
│ Content            │  ← Text size 1-2
│ (y=16-56)          │
│                    │
└────────────────────┘
```

**Text Sizes**:
- Size 1: 6x8 pixels per character (21 chars/line, 8 lines)
- Size 2: 12x16 pixels per character (10 chars/line, 4 lines)

---

## How Sensors Work

### Sensor Update Cycle

1. **Non-blocking Read**: `SensorManager::update()` called every loop
2. **Timing Check**: Only read sensors at intervals (avoid ADC spam)
3. **Processing**: Raw values converted to meaningful units
4. **Storage**: Processed values stored in member variables
5. **Access**: Other modules call getter methods

### Sensor Reading Pattern

```cpp
void SensorManager::update() {
    unsigned long now = millis();
    
    if (now - lastSensorUpdate >= SENSOR_UPDATE_INTERVAL) {
        lastSensorUpdate = now;
        
        // Read and process sensors
        temperature = readThermistor();
        lightLevel = readLDR();
        soundDetected = digitalRead(PIN_SOUND_SENSOR);
        distance = readUltrasonic();
    }
}
```

### Thermistor Reading

**Circuit**: Voltage divider (10kΩ thermistor + 10kΩ resistor)

**Process**:
1. Read ADC (0-1023)
2. Convert to resistance using voltage divider formula
3. Apply Steinhart-Hart equation for temperature

**Calibration**: Adjust coefficients in `config.h` for your thermistor type (NTC 10K is standard).

### LDR Reading

**Circuit**: Voltage divider (LDR + 10kΩ resistor)

**Process**:
1. Read ADC (0-1023)
2. Map to percentage (0% = dark, 100% = bright)
3. Apply smoothing filter (optional)

**Calibration**: 
- Measure ADC in darkness (e.g., 50)
- Measure ADC in bright light (e.g., 900)
- Update `LDR_MIN` and `LDR_MAX` in `config.h`

### Ultrasonic Sensor

**Protocol**:
1. Send 10μs pulse to TRIG pin
2. Measure ECHO pulse width
3. Convert to distance: `distance = duration * 0.034 / 2`

**Range**: 2-400 cm (returns 0 if out of range)

**Presence Detection**: Object detected if distance < threshold (e.g., 50 cm)

---

## How Timing Works

### The millis() Pattern

Arduino's `millis()` returns milliseconds since boot. We use it for non-blocking timing:

```cpp
unsigned long lastUpdate = 0;
const unsigned long interval = 1000; // 1 second

void loop() {
    unsigned long now = millis();
    
    if (now - lastUpdate >= interval) {
        lastUpdate = now;
        // Do periodic task
    }
}
```

### Multiple Timers

Each manager has its own timing variables:

| Manager | Timer | Interval |
|---------|-------|----------|
| PageManager | `lastPageSwitch` | 5000 ms (5 sec) |
| SensorManager | `lastSensorUpdate` | 500 ms (0.5 sec) |
| PomodoroManager | `lastSecond` | 1000 ms (1 sec) |

**Why separate timers?** Allows independent update rates without interference.

### Pomodoro Timer

**Structure**:
```cpp
unsigned long remainingSeconds;  // Time left in current session
unsigned long lastSecond;        // Last time we decremented

void update() {
    if (millis() - lastSecond >= 1000) {
        lastSecond = millis();
        if (remainingSeconds > 0) {
            remainingSeconds--;
        } else {
            // Transition to next state
        }
    }
}
```

**Pause/Resume**: Simply skip decrement when paused.

---

## Adding New Pages

### Step-by-Step Guide

**1. Create the render function** (in `PageManager.cpp`):
```cpp
void renderMyNewPage() {
    displayMgr.clear();
    
    displayMgr.setCursor(0, 0);
    displayMgr.setTextSize(1);
    displayMgr.print("My New Page");
    
    // Add your content here
    displayMgr.setCursor(0, 16);
    displayMgr.print("Content");
    
    displayMgr.display();
}
```

**2. Add to page array** (in `PageManager` constructor):
```cpp
const PageInfo pages[] = {
    {"Home", renderHomePage},
    {"Pomodoro", renderPomodoroPage},
    {"MyPage", renderMyNewPage},  // ← Add here
    // ...
};
```

**3. (Optional) Add page-specific data**:
If your page needs data, add it to `PageManager` class:
```cpp
// In PageManager.h
private:
    int myPageCounter;

// In PageManager.cpp
void renderMyNewPage() {
    displayMgr.print(myPageCounter);
    myPageCounter++; // Example: increment each render
}
```

**Done!** Your page will appear in the auto-cycle rotation.

### Page Design Tips

1. **Keep it simple**: OLED is small (128x64). Don't overcrowd.
2. **Use hierarchy**: Title at top, content below.
3. **Test readability**: Text size 1 is readable; size 2 for emphasis.
4. **Update only when needed**: Cache data, don't recompute every frame.
5. **Handle long text**: Truncate or scroll if text exceeds screen width.

---

## Adding New Sensors

### Step-by-Step Guide

**1. Define pin in `config.h`**:
```cpp
#define PIN_NEW_SENSOR A2
```

**2. Add member variables to `SensorManager.h`**:
```cpp
class SensorManager {
private:
    float newSensorValue;
    
public:
    float getNewSensorValue() const { return newSensorValue; }
};
```

**3. Initialize pin in `SensorManager::begin()`**:
```cpp
void SensorManager::begin() {
    pinMode(PIN_NEW_SENSOR, INPUT);
    // ...
}
```

**4. Read sensor in `SensorManager::update()`**:
```cpp
void SensorManager::update() {
    // ... existing sensor reads ...
    
    newSensorValue = analogRead(PIN_NEW_SENSOR);
    // Apply any processing (mapping, filtering, etc.)
}
```

**5. Use in pages**:
```cpp
void renderNewSensorPage() {
    displayMgr.clear();
    displayMgr.setCursor(0, 0);
    displayMgr.print("Sensor Value:");
    displayMgr.setCursor(0, 16);
    displayMgr.print(sensorMgr.getNewSensorValue());
    displayMgr.display();
}
```

### Sensor Types

**Digital Input** (button, PIR, sound sensor):
```cpp
pinMode(PIN_SENSOR, INPUT);
bool value = digitalRead(PIN_SENSOR);
```

**Analog Input** (potentiometer, LDR, thermistor):
```cpp
pinMode(PIN_SENSOR, INPUT);
int value = analogRead(PIN_SENSOR); // 0-1023
```

**PWM Output** (LED brightness, motor speed):
```cpp
pinMode(PIN_OUTPUT, OUTPUT);
analogWrite(PIN_OUTPUT, 128); // 0-255
```

**Digital Output** (LED, relay, buzzer):
```cpp
pinMode(PIN_OUTPUT, OUTPUT);
digitalWrite(PIN_OUTPUT, HIGH);
```

---

## Adding New Hardware

### Example: Adding a Button for Manual Page Navigation

**1. Hardware**: Connect button between D6 and GND (internal pull-up).

**2. Define pin** (`config.h`):
```cpp
#define PIN_BUTTON D6
```

**3. Create ButtonManager** (new files: `ButtonManager.h/cpp`):
```cpp
// ButtonManager.h
class ButtonManager {
public:
    void begin();
    void update();
    bool wasPressed();
private:
    bool lastState;
    bool pressed;
    unsigned long lastDebounce;
};
```

```cpp
// ButtonManager.cpp
void ButtonManager::begin() {
    pinMode(PIN_BUTTON, INPUT_PULLUP);
    lastState = HIGH;
    pressed = false;
}

void ButtonManager::update() {
    bool currentState = digitalRead(PIN_BUTTON);
    
    // Debounce (wait 50ms after change)
    if (currentState != lastState) {
        lastDebounce = millis();
    }
    
    if (millis() - lastDebounce > 50) {
        if (currentState == LOW && lastState == HIGH) {
            pressed = true; // Falling edge = button pressed
        }
    }
    
    lastState = currentState;
}

bool ButtonManager::wasPressed() {
    bool result = pressed;
    pressed = false; // Clear flag
    return result;
}
```

**4. Integrate in `main.cpp`**:
```cpp
#include "ButtonManager.h"
ButtonManager buttonMgr;

void setup() {
    buttonMgr.begin();
    // ...
}

void loop() {
    buttonMgr.update();
    
    if (buttonMgr.wasPressed()) {
        pageMgr.nextPage(); // Advance to next page
    }
    
    // ...
}
```

**5. Disable auto-cycling** (optional, in `PageManager.cpp`):
```cpp
// Comment out auto-cycle code in PageManager::update()
// if (millis() - lastPageSwitch >= PAGE_SWITCH_INTERVAL) { ... }
```

---

## Extending the Pomodoro System

### Adding Long Breaks

**Goal**: Every 4 work sessions, take a longer break (15 minutes).

**1. Add state** (`PomodoroManager.h`):
```cpp
enum PomodoroState {
    WORK,
    SHORT_BREAK,
    LONG_BREAK,
    PAUSED
};
```

**2. Modify transition logic** (`PomodoroManager.cpp`):
```cpp
void PomodoroManager::update() {
    // ... timer decrement logic ...
    
    if (remainingSeconds == 0) {
        if (state == WORK) {
            sessionNumber++;
            if (sessionNumber % 4 == 0) {
                state = LONG_BREAK;
                remainingSeconds = LONG_BREAK_DURATION;
            } else {
                state = SHORT_BREAK;
                remainingSeconds = SHORT_BREAK_DURATION;
            }
        } else {
            state = WORK;
            remainingSeconds = WORK_DURATION;
        }
    }
}
```

**3. Update page rendering** to show "Long Break" vs "Short Break".

### Adding Session Types

**Goal**: Different sessions (Coding, Study, CTF) with different durations.

**1. Define session types** (`config.h`):
```cpp
enum SessionType {
    CODING,
    STUDY,
    CTF,
    READING,
    RELAX
};

struct SessionConfig {
    const char* name;
    unsigned long workDuration;
    unsigned long breakDuration;
};

const SessionConfig sessions[] = {
    {"Coding", 25 * 60, 5 * 60},
    {"Study", 50 * 60, 10 * 60},
    {"CTF", 30 * 60, 5 * 60},
    // ...
};
```

**2. Add to PomodoroManager**:
```cpp
class PomodoroManager {
private:
    SessionType currentSession;
    
public:
    void setSession(SessionType session);
    const char* getSessionName() const;
};
```

**3. Allow session switching** (via buttons or menu).

---

## Memory Management

### Arduino Nano Constraints

- **Flash**: 32 KB (program storage)
- **SRAM**: 2 KB (runtime variables)
- **EEPROM**: 1 KB (persistent storage)

### RAM Usage Tips

**1. Use `F()` macro for strings**:
```cpp
// BAD: Uses RAM
displayMgr.print("Hello");

// GOOD: Stored in flash
displayMgr.print(F("Hello"));
```

**2. Use `PROGMEM` for large arrays**:
```cpp
const char quote1[] PROGMEM = "Stay focused!";
const char quote2[] PROGMEM = "One task at a time.";

const char* const quotes[] PROGMEM = {quote1, quote2};

// Read from PROGMEM
char buffer[50];
strcpy_P(buffer, (char*)pgm_read_word(&quotes[0]));
```

**3. Monitor RAM usage**:
```cpp
int freeRAM = Utility::getFreeRAM();
Serial.print(F("Free RAM: "));
Serial.println(freeRAM);
```

**Warning Signs**:
- Free RAM < 200 bytes: Risk of stack/heap collision → crashes
- Random resets: Often caused by RAM exhaustion

**Solutions**:
- Move strings to `PROGMEM`
- Reduce buffer sizes
- Use smaller libraries (U8g2 instead of Adafruit_GFX)

---

## Debugging

### Serial Debugging

**1. Enable Serial** (`main.cpp`):
```cpp
void setup() {
    Serial.begin(115200);
    Serial.println(F("DeskHub starting..."));
    // ...
}
```

**2. Add debug prints**:
```cpp
void SensorManager::update() {
    temperature = readThermistor();
    
    #ifdef DEBUG
    Serial.print(F("Temperature: "));
    Serial.println(temperature);
    #endif
}
```

**3. Use `#define DEBUG`** in `config.h` to toggle debug output.

### Common Issues

**Problem**: Display shows nothing
- **Check**: I2C address (run I2C scanner sketch)
- **Check**: Wiring (SDA/SCL correct?)
- **Check**: Power (OLED getting 5V?)

**Problem**: Sensors return 0 or garbage
- **Check**: Pin definitions in `config.h`
- **Check**: Voltage divider resistor values
- **Check**: ADC reference voltage

**Problem**: Timer doesn't count down
- **Check**: `PomodoroManager::update()` called in `loop()`
- **Check**: Timer not paused
- **Check**: `millis()` overflow handling (rare, every 50 days)

**Problem**: Random crashes/resets
- **Check**: Free RAM (use `Utility::getFreeRAM()`)
- **Check**: Stack overflow (deep recursion, large local arrays)
- **Check**: Watchdog timer (if enabled)

---

## Future Expansion Ideas

### Short Term (Easy)

1. **Button Navigation**: Add 2-3 buttons for manual page switching
2. **Buzzer Feedback**: Beep on Pomodoro transitions
3. **Adjustable Timers**: Use potentiometer to set work/break durations
4. **More Quotes**: Expand quote array
5. **Custom Sessions**: Hardcode different session types

### Medium Term (Moderate)

1. **Settings Menu**: Navigate with buttons, adjust timers/thresholds
2. **EEPROM Storage**: Save settings, session counts, total uptime
3. **RTC Module**: Real-time clock for accurate timekeeping
4. **SD Card Logging**: Log sessions, sensor data
5. **Servo Animations**: Visual indicator for Pomodoro state

### Long Term (Complex)

1. **Wireless Connectivity**: ESP32 for WiFi/Bluetooth
2. **Companion App**: Desktop/mobile app for stats, remote control
3. **Web Dashboard**: Display stats on local network
4. **Multi-Device Sync**: Sync Pomodoro across devices
5. **Voice Control**: Add microphone for voice commands

### Hardware Expansion Slots

The current design leaves these pins free:

| Pin | Available For |
|-----|---------------|
| D6, D7, D8, D9, D10, D11, D12, D13 | Digital I/O, buttons, LEDs, servos |
| A2, A3 | Analog sensors, potentiometers |
| A6, A7 | Analog-only inputs (thermistors, LDRs) |

**Example: Adding 3 Navigation Buttons**:
- D6: Up/Next button
- D7: Down/Previous button
- D8: Select/Confirm button

**Example: Adding a Servo**:
- D9: Servo control (PWM)
- External 5V supply for servo (don't power from Arduino)

---

## Best Practices Summary

### Code Style
- ✅ Use descriptive variable names (`temperature`, not `t`)
- ✅ Add comments for complex logic
- ✅ Keep functions short (< 50 lines)
- ✅ Use `const` for values that don't change
- ✅ Use enums for states, not magic numbers

### Architecture
- ✅ One responsibility per class/module
- ✅ Managers communicate through public methods, not global variables
- ✅ Configuration in `config.h`, not scattered in code
- ✅ Non-blocking timing with `millis()`
- ✅ Fail gracefully (check sensor validity, handle errors)

### Performance
- ✅ Use `F()` for string literals
- ✅ Avoid `String` class (uses heap, fragments RAM)
- ✅ Update sensors at reasonable intervals (not every loop)
- ✅ Cache computed values (don't recalculate every frame)
- ✅ Monitor free RAM

### Maintainability
- ✅ Add new features in separate modules
- ✅ Test incrementally (add one feature at a time)
- ✅ Document complex algorithms
- ✅ Use version control (Git)
- ✅ Keep this guide updated

---

## Conclusion

DeskHub is designed to be **your project**—easy to understand, modify, and extend. The modular architecture means you can:

- Add new pages without touching sensor code
- Add new sensors without touching display code
- Swap hardware (different display, MCU) by editing one module
- Experiment with features without breaking existing functionality

**Next Steps**:
1. Upload the firmware and test each page
2. Calibrate sensors (adjust thresholds in `config.h`)
3. Add your own quotes to `QuoteManager`
4. Pick a feature from "Future Expansion Ideas" and implement it
5. Share your modifications!

**Questions?** Review the relevant section above, or check the inline comments in the source code.

**Happy hacking!** 🚀
