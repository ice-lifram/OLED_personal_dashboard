#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include <Arduino.h>
#include "DisplayManager.h"
#include "PomodoroManager.h"
#include "SensorManager.h"
#include "QuoteManager.h"
#include "Utility.h"

enum PageType {
    PAGE_HOME,
    PAGE_POMODORO,
    PAGE_SESSION,
    PAGE_ENVIRONMENT,
    PAGE_QUOTES,
    PAGE_COUNT
};

class PageManager {
public:
    void init(DisplayManager* display, PomodoroManager* pomodoro, SensorManager* sensor, QuoteManager* quote);
    void update();
    void nextPage();
    void setPage(PageType page);
    
private:
    DisplayManager* displayMgr;
    PomodoroManager* pomodoroMgr;
    SensorManager* sensorMgr;
    QuoteManager* quoteMgr;
    
    PageType currentPage;
    unsigned long lastPageCycle;
    unsigned long lastDisplayUpdate;
    
    void drawCurrentPage();
    void drawHomePage();
    void drawPomodoroPage();
    void drawSessionPage();
    void drawEnvironmentPage();
    void drawQuotesPage();
    
    String getLightDescription(int level);
};

#endif
