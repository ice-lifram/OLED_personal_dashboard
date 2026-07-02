#include "PageManager.h"

void PageManager::init(DisplayManager* display, PomodoroManager* pomodoro, SensorManager* sensor, QuoteManager* quote) {
    displayMgr = display;
    pomodoroMgr = pomodoro;
    sensorMgr = sensor;
    quoteMgr = quote;
    
    currentPage = PAGE_HOME;
    lastPageCycle = millis();
    lastDisplayUpdate = 0;
}

void PageManager::update() {
    unsigned long now = millis();
    
    // Auto-cycle pages
    if (now - lastPageCycle >= PAGE_CYCLE_INTERVAL) {
        lastPageCycle = now;
        nextPage();
    }
    
    // Update display
    if (now - lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL) {
        lastDisplayUpdate = now;
        drawCurrentPage();
    }
}

void PageManager::nextPage() {
    currentPage = (PageType)((currentPage + 1) % PAGE_COUNT);
    if (currentPage == PAGE_QUOTES) {
        quoteMgr->nextQuote();
    }
}

void PageManager::setPage(PageType page) {
    currentPage = page;
    lastPageCycle = millis();
}

void PageManager::drawCurrentPage() {
    displayMgr->getU8g2().firstPage();
    do {
        switch (currentPage) {
            case PAGE_HOME:
                drawHomePage();
                break;
            case PAGE_POMODORO:
                drawPomodoroPage();
                break;
            case PAGE_SESSION:
                drawSessionPage();
                break;
            case PAGE_ENVIRONMENT:
                drawEnvironmentPage();
                break;
            case PAGE_QUOTES:
                drawQuotesPage();
                break;
        }
    } while (displayMgr->getU8g2().nextPage());
}

void PageManager::drawHomePage() {
    displayMgr->setFont(u8g2_font_ncenB10_tr);
    displayMgr->drawText(25, 12, "DeskHub");
    
    displayMgr->setFont(u8g2_font_6x10_tf);
    
    String uptime = "Up: " + Utility::formatTime(Utility::getUptime());
    displayMgr->drawText(0, 28, uptime);
    
    String pomodoroMode = "Mode: ";
    switch (pomodoroMgr->getState()) {
        case POMODORO_IDLE:
            pomodoroMode += "Idle";
            break;
        case POMODORO_WORK:
            pomodoroMode += "Work";
            break;
        case POMODORO_BREAK:
            pomodoroMode += "Break";
            break;
    }
    displayMgr->drawText(0, 40, pomodoroMode);
    
    String sessions = "Sessions: " + String(pomodoroMgr->getSessionCount());
    displayMgr->drawText(0, 52, sessions);
}

void PageManager::drawPomodoroPage() {
    displayMgr->setFont(u8g2_font_ncenB08_tr);
    displayMgr->drawText(30, 10, "Pomodoro");
    
    displayMgr->setFont(u8g2_font_6x10_tf);
    
    String state = "State: ";
    switch (pomodoroMgr->getState()) {
        case POMODORO_IDLE:
            state += "Idle";
            break;
        case POMODORO_WORK:
            state += "Work";
            break;
        case POMODORO_BREAK:
            state += "Break";
            break;
    }
    if (pomodoroMgr->isPaused()) {
        state += " (Paused)";
    }
    displayMgr->drawText(0, 24, state);
    
    if (pomodoroMgr->getState() != POMODORO_IDLE) {
        String remaining = "Left: " + Utility::formatTime(pomodoroMgr->getTimeRemaining());
        displayMgr->drawText(0, 38, remaining);
        
        String elapsed = "Done: " + Utility::formatTime(pomodoroMgr->getTimeElapsed());
        displayMgr->drawText(0, 52, elapsed);
    }
}

void PageManager::drawSessionPage() {
    displayMgr->setFont(u8g2_font_ncenB08_tr);
    displayMgr->drawText(35, 10, "Session");
    
    displayMgr->setFont(u8g2_font_6x10_tf);
    
    // Hardcoded activity for now
    displayMgr->drawText(0, 28, "Activity:");
    displayMgr->setFont(u8g2_font_ncenB08_tr);
    displayMgr->drawText(25, 45, "Coding");
    
    displayMgr->setFont(u8g2_font_6x10_tf);
    displayMgr->drawText(0, 60, "Sessions: " + String(pomodoroMgr->getSessionCount()));
}

void PageManager::drawEnvironmentPage() {
    displayMgr->setFont(u8g2_font_ncenB08_tr);
    displayMgr->drawText(20, 10, "Environment");
    
    displayMgr->setFont(u8g2_font_6x10_tf);
    
    String temp = "Temp: " + String(sensorMgr->getTemperature(), 1) + "C";
    displayMgr->drawText(0, 26, temp);
    
    int lightLevel = sensorMgr->getLightLevel();
    String light = "Light: " + String(lightLevel);
    displayMgr->drawText(0, 40, light);
    displayMgr->drawText(0, 52, getLightDescription(lightLevel));
 
}


void PageManager::drawQuotesPage() {
    displayMgr->setFont(u8g2_font_ncenB08_tr);
    displayMgr->drawText(35, 10, "Quotes");
    
    displayMgr->setFont(u8g2_font_6x10_tf);
    
    const char* quote = quoteMgr->getCurrentQuote();
    
    // Simple word wrap for quotes
    String quoteStr = String(quote);
    int lineY = 35;
    int maxWidth = 120;
    int startIdx = 0;
    
    while (startIdx < quoteStr.length()) {
        int endIdx = startIdx + 20;
        if (endIdx >= quoteStr.length()) {
            endIdx = quoteStr.length();
        } else {
            // Find last space
            while (endIdx > startIdx && quoteStr.charAt(endIdx) != ' ') {
                endIdx--;
            }
            if (endIdx == startIdx) {
                endIdx = startIdx + 20;
            }
        }
        
        String line = quoteStr.substring(startIdx, endIdx);
        displayMgr->drawText(5, lineY, line);
        lineY += 12;
        startIdx = endIdx + 1;
        
        if (lineY > 60) break;
    }
}

String PageManager::getLightDescription(int level) {
    if (level < 100) {
        return "Very dark";
    } else if (level < 300) {
        return "Dark";
    } else if (level < 600) {
        return "Dim";
    } else if (level < 800) {
        return "Bright";
    } else {
        return "Very bright";
    }
}
