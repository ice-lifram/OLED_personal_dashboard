#ifndef UTILITY_H
#define UTILITY_H

#include <Arduino.h>

class Utility {
public:
    static unsigned long getUptime();
    static int getFreeRAM();
    static String formatTime(unsigned long milliseconds);
};

#endif
