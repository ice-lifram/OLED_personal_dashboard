#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <U8g2lib.h>
#include "config.h"

class DisplayManager {
public:
    void init();
    void clear();
    void showSplash();
    void beginDraw();
    void endDraw();
    
    void drawText(int x, int y, const char* text);
    void drawText(int x, int y, const String& text);
    void setFont(const uint8_t* font);
    
    U8G2_SSD1306_128X64_NONAME_1_HW_I2C& getU8g2() { return u8g2; }
    
private:
    U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2 = U8G2_SSD1306_128X64_NONAME_1_HW_I2C(U8G2_R0, U8X8_PIN_NONE);
};

#endif
