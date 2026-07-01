/*
==========================================================
 display.cpp
----------------------------------------------------------
 OLED Display Module

 Responsibilities:
    - OLED initialization
    - Screen rendering
    - Text utilities
    - Drawing utilities
    - Splash screen

 NOTE:
    This file contains a framework and placeholders.
    Expand functionality incrementally.
==========================================================
*/

#include "display.h"
#include "config.h"
#include "pages.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



//----------------------------------------------------------
// Private Display Instance
//----------------------------------------------------------

Adafruit_SSD1306 oled(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    OLED_RESET_PIN
);



//----------------------------------------------------------
// Initialization
//----------------------------------------------------------

void initDisplay()
{
    if (!oled.begin(
            SSD1306_SWITCHCAPVCC,
            OLED_I2C_ADDRESS))
    {
        Serial.println(F("OLED initialization failed."));

        while (true)
        {
            delay(100);
        }
    }

    oled.clearDisplay();
    oled.display();

    Serial.println(F("OLED initialized."));
}



//----------------------------------------------------------
// Splash Screen
//----------------------------------------------------------

void showSplashScreen()
{
    oled.clearDisplay();

    oled.setTextSize(2);
    oled.setTextColor(SSD1306_WHITE);

    drawCenteredText(
        "NANO",
        16,
        2
    );

    drawCenteredText(
        "DASHBOARD",
        40,
        1
    );

    oled.display();
}



//----------------------------------------------------------
// Dashboard Rendering
//----------------------------------------------------------

void clearDisplayBuffer()
{
    oled.clearDisplay();
}



void updateDisplay()
{
    oled.display();
}



void drawCurrentPage()
{
    /*
        NOTE

        This function acts as a bridge between
        the display module and pages module.

        The actual page content should be drawn
        inside pages.cpp.
    */

    clearDisplayBuffer();

    renderCurrentPage();

    updateDisplay();
}



//----------------------------------------------------------
// Text Utilities
//----------------------------------------------------------

void drawCenteredText(
    const String& text,
    int y,
    uint8_t textSize)
{
    oled.setTextSize(textSize);

    int16_t x1;
    int16_t y1;

    uint16_t width;
    uint16_t height;

    oled.getTextBounds(
        text,
        0,
        0,
        &x1,
        &y1,
        &width,
        &height
    );

    int x =
        (SCREEN_WIDTH - width) / 2;

    oled.setCursor(x, y);
    oled.print(text);
}



void drawLeftText(
    const String& text,
    int x,
    int y,
    uint8_t textSize)
{
    oled.setTextSize(textSize);

    oled.setCursor(x, y);

    oled.print(text);
}



void drawRightText(
    const String& text,
    int y,
    uint8_t textSize)
{
    oled.setTextSize(textSize);

    int16_t x1;
    int16_t y1;

    uint16_t width;
    uint16_t height;

    oled.getTextBounds(
        text,
        0,
        0,
        &x1,
        &y1,
        &width,
        &height
    );

    int x =
        SCREEN_WIDTH - width;

    oled.setCursor(x, y);

    oled.print(text);
}



//----------------------------------------------------------
// Drawing Utilities
//----------------------------------------------------------

void drawHorizontalLine(int y)
{
    oled.drawLine(
        0,
        y,
        SCREEN_WIDTH,
        y,
        SSD1306_WHITE
    );
}



void drawFrame(
    int x,
    int y,
    int width,
    int height)
{
    oled.drawRect(
        x,
        y,
        width,
        height,
        SSD1306_WHITE
    );
}



void drawProgressBar(
    int x,
    int y,
    int width,
    int height,
    uint8_t percent)
{
    percent = constrain(percent, 0, 100);

    drawFrame(
        x,
        y,
        width,
        height
    );

    int fillWidth =
        map(
            percent,
            0,
            100,
            0,
            width - 2
        );

    oled.fillRect(
        x + 1,
        y + 1,
        fillWidth,
        height - 2,
        SSD1306_WHITE
    );
}



//----------------------------------------------------------
// Status Icons
//----------------------------------------------------------

void drawStatusIcons()
{
    /*
        TODO

        Future icons:

        - Battery
        - Bluetooth
        - Wi-Fi
        - Sensor health
        - Alert indicator
    */
}



//----------------------------------------------------------
// Future Graphics
//----------------------------------------------------------

void playAnimation()
{
    /*
        TODO

        Placeholder for future animations.
    */
}



void transitionScreen()
{
    /*
        TODO

        Placeholder for future transitions.
    */
}



//----------------------------------------------------------
// Debug
//----------------------------------------------------------

void debugDisplay()
{
    Serial.println();
    Serial.println(F("=== DISPLAY DEBUG ==="));

    Serial.print(F("Width: "));
    Serial.println(SCREEN_WIDTH);

    Serial.print(F("Height: "));
    Serial.println(SCREEN_HEIGHT);

    Serial.print(F("I2C Address: "));
    Serial.println(OLED_I2C_ADDRESS, HEX);

    Serial.println(F("====================="));
}
