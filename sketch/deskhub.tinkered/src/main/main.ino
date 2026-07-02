#include <Arduino.h>
#include "config.h"
#include "DisplayManager.h"
#include "PageManager.h"
#include "PomodoroManager.h"
#include "SensorManager.h"
#include "QuoteManager.h"

DisplayManager displayMgr;
PomodoroManager pomodoroMgr;
SensorManager sensorMgr;
QuoteManager quoteMgr;
PageManager pageMgr;

void setup() {
    Serial.begin(115200);
    
    displayMgr.init();
    displayMgr.showSplash();
    delay(2000);
    
    pomodoroMgr.init();
    sensorMgr.init();
    quoteMgr.init();
    
    pageMgr.init(&displayMgr, &pomodoroMgr, &sensorMgr, &quoteMgr);
    
    // Auto-start pomodoro for demo
    pomodoroMgr.start();
}

void loop() {
    pomodoroMgr.update();
    sensorMgr.update();
    pageMgr.update();
}
