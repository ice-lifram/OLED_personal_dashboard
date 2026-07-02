#ifndef POMODORO_MANAGER_H
#define POMODORO_MANAGER_H

#include <Arduino.h>
#include "config.h"

enum PomodoroState {
    POMODORO_IDLE,
    POMODORO_WORK,
    POMODORO_BREAK
};

class PomodoroManager {
public:
    void init();
    void update();
    void start();
    void pause();
    void resume();
    void reset();
    
    PomodoroState getState() const { return state; }
    unsigned long getTimeRemaining() const;
    unsigned long getTimeElapsed() const;
    bool isPaused() const { return paused; }
    uint16_t getSessionCount() const { return sessionCount; }
    
private:
    PomodoroState state;
    unsigned long startTime;
    unsigned long pauseTime;
    unsigned long elapsedBeforePause;
    bool paused;
    uint16_t sessionCount;
    
    void switchToBreak();
    void switchToWork();
    void playBuzzer();
};

#endif
