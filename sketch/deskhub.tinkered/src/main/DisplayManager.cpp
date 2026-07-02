#include "DisplayManager.h"

void DisplayManager::init() {
    u8g2.begin();
    u8g2.setFont(u8g2_font_6x10_tf);
}

void DisplayManager::clear() {
    u8g2.clearBuffer();
}

void DisplayManager::showSplash() {
    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_ncenB14_tr);
        u8g2.drawStr(20, 30, "DeskHub");
        u8g2.setFont(u8g2_font_6x10_tf);
        u8g2.drawStr(30, 50, "Starting...");
    } while (u8g2.nextPage());
}

void DisplayManager::beginDraw() {
    u8g2.firstPage();
}

void DisplayManager::endDraw() {
    // U8g2 page mode handles this in the do-while loop
}

void DisplayManager::drawText(int x, int y, const char* text) {
    u8g2.drawStr(x, y, text);
}

void DisplayManager::drawText(int x, int y, const String& text) {
    u8g2.drawStr(x, y, text.c_str());
}

void DisplayManager::setFont(const uint8_t* font) {
    u8g2.setFont(font);
}
