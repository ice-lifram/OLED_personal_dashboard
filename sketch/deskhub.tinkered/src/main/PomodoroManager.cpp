#include "PomodoroManager.h"

void PomodoroManager::init() {
    state = POMODORO_IDLE;
    startTime = 0;
    pauseTime = 0;
    elapsedBeforePause = 0;
    paused = false;
    sessionCount = 0;
    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);
}

void PomodoroManager::update() {
    if (state == POMODORO_IDLE || paused) {
        return;
    }
    
    unsigned long elapsed = millis() - startTime + elapsedBeforePause;
    
    if (state == POMODORO_WORK && elapsed >= POMODORO_WORK_DURATION) {
        switchToBreak();
    } else if (state == POMODORO_BREAK && elapsed >= POMODORO_BREAK_DURATION) {
        switchToWork();
    }
}

void PomodoroManager::start() {
    state = POMODORO_WORK;
    startTime = millis();
    elapsedBeforePause = 0;
    paused = false;
    sessionCount = 0;
}

void PomodoroManager::pause() {
    if (state != POMODORO_IDLE && !paused) {
        paused = true;
        pauseTime = millis();
        elapsedBeforePause += (pauseTime - startTime);
    }
}

void PomodoroManager::resume() {
    if (paused) {
        paused = false;
        startTime = millis();
    }
}

void PomodoroManager::reset() {
    state = POMODORO_IDLE;
    startTime = 0;
    pauseTime = 0;
    elapsedBeforePause = 0;
    paused = false;
    digitalWrite(PIN_BUZZER, LOW);
}

unsigned long PomodoroManager::getTimeRemaining() const {
    if (state == POMODORO_IDLE) {
        return 0;
    }
    
    unsigned long elapsed = paused ? elapsedBeforePause : (millis() - startTime + elapsedBeforePause);
    unsigned long duration = (state == POMODORO_WORK) ? POMODORO_WORK_DURATION : POMODORO_BREAK_DURATION;
    
    if (elapsed >= duration) {
        return 0;
    }
    return duration - elapsed;
}

unsigned long PomodoroManager::getTimeElapsed() const {
    if (state == POMODORO_IDLE) {
        return 0;
    }
    return paused ? elapsedBeforePause : (millis() - startTime + elapsedBeforePause);
}

void PomodoroManager::switchToBreak() {
    state = POMODORO_BREAK;
    startTime = millis();
    elapsedBeforePause = 0;
    sessionCount++;
    playBuzzer();
}

void PomodoroManager::switchToWork() {
    state = POMODORO_WORK;
    startTime = millis();
    elapsedBeforePause = 0;
    playBuzzer();
}

void PomodoroManager::playBuzzer() {
    tone(PIN_BUZZER, 1000, 200);
}
