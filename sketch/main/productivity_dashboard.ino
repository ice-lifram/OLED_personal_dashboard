/*
==========================================================
 Nano Productivity Dashboard
----------------------------------------------------------
 Main Application Entry Point

 Purpose:
    - Initialize all modules
    - Coordinate program execution
    - Keep loop() lightweight
    - Avoid placing feature logic here

 Author:
    Ice Lifram

 Notes:
    This file intentionally contains only the high-level
    application flow. Individual features belong in their
    respective modules.
==========================================================
*/

#include "config.h"

#include "display.h"
#include "sensors.h"
#include "buttons.h"
#include "pages.h"



//----------------------------------------------------------
// Global Timing
//----------------------------------------------------------

unsigned long previousSensorRead = 0;
unsigned long previousDisplayRefresh = 0;



//----------------------------------------------------------
// setup()
//----------------------------------------------------------

void setup()
{
    // Initialize Serial Monitor
    Serial.begin(SERIAL_BAUDRATE);

    // Optional startup delay
    delay(STARTUP_DELAY);

    // Initialize hardware modules
    initDisplay();
    initSensors();
    initButtons();

    // Draw splash screen
    showSplashScreen();

    // Optional startup delay so splash can be seen
    delay(SPLASH_DURATION);

    // Prepare first page
    setCurrentPage(PAGE_HOME);
}



//----------------------------------------------------------
// loop()
//----------------------------------------------------------

void loop()
{
    updateButtons();

    handleNavigation();

    updateSensors();

    refreshDashboard();
}



//----------------------------------------------------------
// updateSensors()
//----------------------------------------------------------

void updateSensors()
{
    if (millis() - previousSensorRead >= SENSOR_INTERVAL)
    {
        previousSensorRead = millis();

        readAllSensors();
    }
}



//----------------------------------------------------------
// refreshDashboard()
//----------------------------------------------------------

void refreshDashboard()
{
    if (millis() - previousDisplayRefresh >= DISPLAY_INTERVAL)
    {
        previousDisplayRefresh = millis();

        drawCurrentPage();
    }
}



//----------------------------------------------------------
// handleNavigation()
//----------------------------------------------------------

void handleNavigation()
{
    /*
        Future responsibilities:

        - Next page

        - Previous page

        - Home shortcut

        - Long press

        - Double click

        - Settings menu

        - Sleep mode

    */

    if (nextPagePressed())
    {
        nextPage();
    }

    if (previousPagePressed())
    {
        previousPage();
    }
}
