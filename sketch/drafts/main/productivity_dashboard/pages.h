/*
==========================================================
 pages.h
----------------------------------------------------------
 Dashboard Page Manager

 Responsibilities
    - Define dashboard pages
    - Handle page navigation
    - Render the active page

 NOTE:
    This module decides WHAT is shown.
    It does not perform low-level drawing.
==========================================================
*/

#ifndef PAGES_H
#define PAGES_H

#include <Arduino.h>



//----------------------------------------------------------
// Dashboard Pages
//----------------------------------------------------------

enum DashboardPage
{
    PAGE_HOME = 0,

    PAGE_ENVIRONMENT,

    PAGE_HEAT_INDEX,

    PAGE_SYSTEM,

    PAGE_DEBUG,

    PAGE_COUNT
};



//----------------------------------------------------------
// Page Navigation
//----------------------------------------------------------

/*
    Set the currently active page.
*/
void setCurrentPage(DashboardPage page);

/*
    Returns the active page.
*/
DashboardPage getCurrentPage();

/*
    Advance to the next page.

    Automatically wraps around.
*/
void nextPage();

/*
    Return to the previous page.

    Automatically wraps around.
*/
void previousPage();



//----------------------------------------------------------
// Rendering
//----------------------------------------------------------

/*
    Render whichever page is currently active.

    Called by display.cpp.
*/
void renderCurrentPage();



//----------------------------------------------------------
// Individual Pages
//----------------------------------------------------------

/*
    Home / Welcome screen.
*/
void renderHomePage();

/*
    Environmental measurements.
*/
void renderEnvironmentPage();

/*
    Heat Index page.
*/
void renderHeatIndexPage();

/*
    System diagnostics.
*/
void renderSystemPage();

/*
    Development / debugging page.
*/
void renderDebugPage();



//----------------------------------------------------------
// Optional Future Pages
//----------------------------------------------------------

/*
    Reserved expansion points.

    Uncomment or extend when needed.
*/

// void renderClockPage();

// void renderPomodoroPage();

// void renderSettingsPage();

// void renderAirQualityPage();

// void renderStatisticsPage();



//----------------------------------------------------------
// Page Information
//----------------------------------------------------------

/*
    Returns the total number
    of implemented pages.
*/
uint8_t getPageCount();



//----------------------------------------------------------
// Debug
//----------------------------------------------------------

/*
    Print page information to Serial.
*/
void printPageReport();

#endif
