#include "Utility.h"

unsigned long Utility::getUptime() {
    return millis();
}

int Utility::getFreeRAM() {
    extern int __heap_start, *__brkval;
    int v;
    return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

String Utility::formatTime(unsigned long milliseconds) {
    unsigned long seconds = milliseconds / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    
    seconds %= 60;
    minutes %= 60;
    
    String result = "";
    if (hours > 0) {
        result += String(hours) + "h ";
    }
    if (minutes > 0 || hours > 0) {
        result += String(minutes) + "m ";
    }
    result += String(seconds) + "s";
    
    return result;
}
