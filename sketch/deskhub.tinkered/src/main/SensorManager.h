#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include "config.h"

class SensorManager {
public:
    void init();
    void update();
    
    float getTemperature() const { return temperature; }
    int getLightLevel() const { return lightLevel; }
    bool getSoundDetected() const { return soundDetected; }
    float getDistance() const { return distance; }
    
private:
    float temperature;
    int lightLevel;
    bool soundDetected;
    float distance;
    
    unsigned long lastSensorRead;
    
    void readThermistor();
    void readLDR();
    void readSoundSensor();
    void readUltrasonic();
};

#endif
