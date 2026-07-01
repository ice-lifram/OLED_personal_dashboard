/*
==========================================================
 display.h
----------------------------------------------------------
 OLED Display Module Interface

 Responsibilities
    - OLED initialization
    - Screen updates
    - Splash screen
    - Drawing dashboard pages
    - Utility drawing functions

 NOTE:
    Implementation belongs in display.cpp.
==========================================================
*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>



//----------------------------------------------------------
// Initialization
//----------------------------------------------------------

/*
    Initializes the OLED display.

    Call once from setup().
*/
void initDisplay();



//----------------------------------------------------------
// Splash Screen
//----------------------------------------------------------

/*
    Displays the project logo or startup screen.

    Called once after initialization.
*/
void showSplashScreen();



//----------------------------------------------------------
// Dashboard Rendering
//----------------------------------------------------------

/*
    Clears the display buffer.

    Does NOT immediately update the OLED.
*/
void clearDisplayBuffer();

/*
    Sends the current display buffer to the OLED.

    Call after drawing operations.
*/
void updateDisplay();

/*
    Draws the currently selected dashboard page.

    The actual page selection is handled by pages.cpp.
*/
void drawCurrentPage();



//----------------------------------------------------------
// Text Utilities
//----------------------------------------------------------

/*
    Draw centered text.

    Future implementation:
        - Automatic centering
        - Adjustable text size
*/
void drawCenteredText(
    const String& text,
    int y,
    uint8_t textSize = 1
);

/*
    Draw left-aligned text.

    Useful for labels and sensor values.
*/
void drawLeftText(
    const String& text,
    int x,
    int y,
    uint8_t textSize = 1
);

/*
    Draw right-aligned text.

    Useful for values and timestamps.
*/
void drawRightText(
    const String& text,
    int y,
    uint8_t textSize = 1
);



//----------------------------------------------------------
// Drawing Utilities
//----------------------------------------------------------

/*
    Draw a horizontal divider.
*/
void drawHorizontalLine(int y);

/*
    Draw a framed box.
*/
void drawFrame(
    int x,
    int y,
    int width,
    int height
);

/*
    Draw a progress bar.

    Future uses:
        - Pomodoro timer
        - Humidity gauge
        - Brightness
        - Battery level
*/
void drawProgressBar(
    int x,
    int y,
    int width,
    int height,
    uint8_t percent
);



//----------------------------------------------------------
// Status Icons
//----------------------------------------------------------

/*
    Reserved for future icon drawing.

    Examples:

    Wi-Fi
    Battery
    Sensor status
    Bluetooth
    Warning icons
*/
void drawStatusIcons();



//----------------------------------------------------------
// Future Graphics
//----------------------------------------------------------

/*
    Future animation handler.
*/
void playAnimation();

/*
    Future screen transition.
*/
void transitionScreen();



//----------------------------------------------------------
// Debug
//----------------------------------------------------------

/*
    Prints display information to Serial.

    Helpful while debugging.
*/
void debugDisplay();



#endif
