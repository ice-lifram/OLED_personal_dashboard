#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_ADDR 0x3C
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ----------------------------
// Dashboard Settings
// ----------------------------
const uint8_t PAGE_COUNT = 4;
const unsigned long PAGE_DURATION = 10000;

const unsigned long WORK_TIME = 25UL * 60UL;
const unsigned long BREAK_TIME = 5UL * 60UL;

// ----------------------------

uint8_t currentPage = 0;

unsigned long uptimeSeconds = 0;

unsigned long pageTimer = 0;
unsigned long secondTimer = 0;

bool screenNeedsUpdate = true;

// Pomodoro
bool workMode = true;
unsigned long timerRemaining = WORK_TIME;

extern int __heap_start, *__brkval;

int freeMemory()
{
    int v;
    return (int)&v - (__brkval == 0 ?
                     (int)&__heap_start :
                     (int)__brkval);
}

// -------------------------------------------------

void centerText(const char *text, int y, uint8_t size)
{
    display.setTextSize(size);
    display.setTextColor(SSD1306_WHITE);

    int16_t x1, y1;
    uint16_t w, h;

    display.getTextBounds(text, 0, y, &x1, &y1, &w, &h);

    display.setCursor((SCREEN_WIDTH - w) / 2, y);
    display.print(text);
}

// -------------------------------------------------

void drawClock()
{
    display.clearDisplay();

    unsigned long h = (uptimeSeconds / 3600) % 24;
    unsigned long m = (uptimeSeconds / 60) % 60;
    unsigned long s = uptimeSeconds % 60;

    char buf[16];
    sprintf(buf, "%02lu:%02lu:%02lu", h, m, s);

    centerText(buf, 0, 2);

    centerText("System Uptime", 22, 1);

    display.display();
}

// -------------------------------------------------

void drawStatus()
{
    display.clearDisplay();

    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("RAM : ");
    display.print(freeMemory());
    display.print(" B");

    display.setCursor(0, 10);
    display.print("Uptime:");
    display.print(uptimeSeconds);
    display.print("s");

    display.setCursor(0, 20);
    display.print("Mode : ");
    display.print(workMode ? "WORK" : "BREAK");

    display.display();
}

// -------------------------------------------------

void drawPomodoro()
{
    display.clearDisplay();

    unsigned long mm = timerRemaining / 60;
    unsigned long ss = timerRemaining % 60;

    char buf[10];
    sprintf(buf, "%02lu:%02lu", mm, ss);

    centerText(buf, 0, 2);

    centerText(workMode ? "Focus Time" : "Break Time", 18, 1);

    int barWidth = map(timerRemaining,
                       0,
                       workMode ? WORK_TIME : BREAK_TIME,
                       0,
                       126);

    barWidth = constrain(barWidth, 0, 126);

    display.drawRect(0, 24, 128, 8, SSD1306_WHITE);
    display.fillRect(1, 25, barWidth, 6, SSD1306_WHITE);

    display.display();
}

// -------------------------------------------------

void drawSplash()
{
    display.clearDisplay();

    centerText("Arduino Nano", 2, 1);
    centerText("OLED Dashboard", 12, 1);
    centerText("Ready", 22, 1);

    display.display();
}

// -------------------------------------------------

void drawCurrentPage()
{
    switch (currentPage)
    {
        case 0:
            drawClock();
            break;

        case 1:
            drawStatus();
            break;

        case 2:
            drawPomodoro();
            break;

        case 3:
            drawSplash();
            break;
    }
}

// -------------------------------------------------

void setup()
{
    Wire.begin();

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
    {
        while (true);
    }

    display.clearDisplay();

    centerText("Booting...", 8, 2);

    display.display();

    delay(1000);

    pageTimer = millis();
    secondTimer = millis();

    screenNeedsUpdate = true;
}

// -------------------------------------------------

void loop()
{
    unsigned long now = millis();

    // One-second tasks
    if (now - secondTimer >= 1000)
    {
        secondTimer += 500;

        uptimeSeconds++;

        if (timerRemaining > 0)
        {
            timerRemaining--;
        }

        if (timerRemaining == 0)
        {
            if (workMode)
            {
                workMode = false;
                timerRemaining = BREAK_TIME;
            }
            else
            {
                workMode = true;
                timerRemaining = WORK_TIME;
            }
        }

        screenNeedsUpdate = true;
    }

    // Page rotation
    if (now - pageTimer >= PAGE_DURATION)
    {
        pageTimer += PAGE_DURATION;

        currentPage++;

        if (currentPage >= PAGE_COUNT)
            currentPage = 0;

        screenNeedsUpdate = true;
    }

    // Refresh only when necessary
    if (screenNeedsUpdate)
    {
        drawCurrentPage();
        screenNeedsUpdate = false;
    }
}
