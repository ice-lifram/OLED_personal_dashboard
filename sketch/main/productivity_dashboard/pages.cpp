/*
==========================================================
 pages.cpp
----------------------------------------------------------
 Dashboard Page Manager

 Responsibilities
    - Manage current page
    - Navigate between pages
    - Render dashboard pages

 NOTE:
    This module decides WHAT to display.
    Drawing primitives are provided by display.cpp.
==========================================================
*/

#include "pages.h"

#include "display.h"
#include "sensors.h"
#include "config.h"



//----------------------------------------------------------
// Current Page
//----------------------------------------------------------

static DashboardPage currentPage = PAGE_HOME;



//----------------------------------------------------------
// Navigation
//----------------------------------------------------------

void setCurrentPage(DashboardPage page)
{
    if (page < PAGE_COUNT)
    {
        currentPage = page;
    }
}



DashboardPage getCurrentPage()
{
    return currentPage;
}



void nextPage()
{
    currentPage =
        static_cast<DashboardPage>(
            (currentPage + 1) % PAGE_COUNT
        );
}



void previousPage()
{
    if (currentPage == PAGE_HOME)
    {
        currentPage =
            static_cast<DashboardPage>(
                PAGE_COUNT - 1
            );
    }
    else
    {
        currentPage =
            static_cast<DashboardPage>(
                currentPage - 1
            );
    }
}



//----------------------------------------------------------
// Rendering Dispatcher
//----------------------------------------------------------

void renderCurrentPage()
{
    switch (currentPage)
    {
        case PAGE_HOME:
            renderHomePage();
            break;

        case PAGE_ENVIRONMENT:
            renderEnvironmentPage();
            break;

        case PAGE_HEAT_INDEX:
            renderHeatIndexPage();
            break;

        case PAGE_SYSTEM:
            renderSystemPage();
            break;

        case PAGE_DEBUG:
            renderDebugPage();
            break;

        default:
            renderHomePage();
            break;
    }
}



//----------------------------------------------------------
// Home Page
//----------------------------------------------------------

void renderHomePage()
{
    drawCenteredText(
        "Nano Dashboard",
        8,
        1
    );

    drawHorizontalLine(20);

    drawCenteredText(
        "Ready",
        34,
        2
    );

    drawCenteredText(
        "Page 1",
        56,
        1
    );
}



//----------------------------------------------------------
// Environment Page
//----------------------------------------------------------

void renderEnvironmentPage()
{
    SensorData data = getSensorData();

    drawLeftText(
        "Temperature",
        0,
        0
    );

    drawLeftText(
        String(data.temperature) + " C",
        0,
        14,
        2
    );

    drawLeftText(
        "Humidity",
        0,
        42
    );

    drawRightText(
        String(data.humidity) + "%",
        54
    );
}



//----------------------------------------------------------
// Heat Index Page
//----------------------------------------------------------

void renderHeatIndexPage()
{
    SensorData data = getSensorData();

    drawCenteredText(
        "Heat Index",
        0
    );

    drawHorizontalLine(12);

    drawCenteredText(
        String(data.heatIndex) + " C",
        26,
        2
    );

    /*
        TODO

        Future:

        Comfortable

        Warm

        Hot

        Dangerous
    */
}



//----------------------------------------------------------
// System Page
//----------------------------------------------------------

void renderSystemPage()
{
    drawLeftText(
        "System",
        0,
        0
    );

    drawHorizontalLine(10);

    drawLeftText(
        "DHT:",
        0,
        18
    );

    drawRightText(
        isDHTAvailable() ?
        "OK" :
        "FAIL",
        18
    );

    drawLeftText(
        "Sensors",
        0,
        34
    );

    drawRightText(
        sensorsHealthy() ?
        "READY" :
        "ERROR",
        34
    );

    /*
        Future additions:

        - Uptime

        - Free RAM

        - Build version

        - CPU frequency
    */
}



//----------------------------------------------------------
// Debug Page
//----------------------------------------------------------

void renderDebugPage()
{
    SensorData data = getSensorData();

    drawLeftText(
        "DEBUG",
        0,
        0
    );

    drawHorizontalLine(10);

    drawLeftText(
        "T:",
        0,
        18
    );

    drawRightText(
        String(data.temperature),
        18
    );

    drawLeftText(
        "H:",
        0,
        30
    );

    drawRightText(
        String(data.humidity),
        30
    );

    drawLeftText(
        "HI:",
        0,
        42
    );

    drawRightText(
        String(data.heatIndex),
        42
    );
}



//----------------------------------------------------------
// Page Information
//----------------------------------------------------------

uint8_t getPageCount()
{
    return PAGE_COUNT;
}



//----------------------------------------------------------
// Debug
//----------------------------------------------------------

void printPageReport()
{
    Serial.println();

    Serial.println(F("===== PAGE REPORT ====="));

    Serial.print(F("Current Page: "));
    Serial.println(currentPage);

    Serial.print(F("Total Pages : "));
    Serial.println(PAGE_COUNT);

    Serial.println(F("======================="));
}
