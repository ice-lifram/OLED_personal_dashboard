#ifndef QUOTE_MANAGER_H
#define QUOTE_MANAGER_H

#include <Arduino.h>

class QuoteManager {
public:
    void init();
    const char* getCurrentQuote();
    void nextQuote();
    
private:
    uint8_t currentIndex;
    static const char* quotes[];
    static const uint8_t quoteCount;
};

#endif
