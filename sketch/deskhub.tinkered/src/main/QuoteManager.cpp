#include "QuoteManager.h"

const char* QuoteManager::quotes[] = {
    "Code is poetry",
    "Stay focused",
    "One step at a time",
    "Debug your mind",
    "Keep learning",
    "Build something",
    "Break the routine",
    "Think different",
    "Iterate & improve",
    "Ship it!"
};

const uint8_t QuoteManager::quoteCount = sizeof(quotes) / sizeof(quotes[0]);

void QuoteManager::init() {
    currentIndex = 0;
}

const char* QuoteManager::getCurrentQuote() {
    return quotes[currentIndex];
}

void QuoteManager::nextQuote() {
    currentIndex = (currentIndex + 1) % quoteCount;
}
