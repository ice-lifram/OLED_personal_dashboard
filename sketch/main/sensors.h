/*
==========================================================
 sensors.h
----------------------------------------------------------
 Sensor Module Interface

 Responsibilities
    - Initialize all sensors
    - Read sensor values
    - Validate readings
    - Provide processed data
    - Future sensor expansion

 NOTE:
    Only function declarations belong here.
==========================================================
*/

#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>



//----------------------------------------------------------
// Sensor Data Structure
//----------------------------------------------------------

/*
    Centralized container for all dashboard readings.

    Future sensors can simply be appended here without
    changing the rest of the project architecture.
*/

struct SensorData
{
    // Environmental
    float temperature;
    float humidity;
    float heatIndex;

    // Optional Future Sensors
    float airQuality;
    float lightLevel;
    float pressure;
    float altitude;

    // Status Flags
    bool dhtAvailable;
    bool mq135Available;
    bool rtcAvailable;
};



//----------------------------------------------------------
// Initialization
//----------------------------------------------------------

/*
    Initialize all installed sensors.

    Called once from setup().
*/
void initSensors();



//----------------------------------------------------------
// Reading Functions
//----------------------------------------------------------

/*
    Reads every available sensor.

    Called periodically from loop().
*/
void readAllSensors();



/*
    Reads the DHT sensor only.

    Useful for testing and future scheduling.
*/
void readDHT();



/*
    Placeholder for future sensors.
*/

void readMQ135();

void readLDR();

void readBMP280();

void readRTC();



//----------------------------------------------------------
// Data Access
//----------------------------------------------------------

/*
    Returns a read-only copy of all current values.
*/
SensorData getSensorData();



//----------------------------------------------------------
// Convenience Getters
//----------------------------------------------------------

float getTemperature();

float getHumidity();

float getHeatIndex();

float getAirQuality();

float getLightLevel();

float getPressure();

float getAltitude();



//----------------------------------------------------------
// Sensor Status
//----------------------------------------------------------

bool isDHTAvailable();

bool isMQ135Available();

bool isRTCAvailable();



//----------------------------------------------------------
// Validation
//----------------------------------------------------------

/*
    Returns true if all required sensors
    are operating correctly.
*/
bool sensorsHealthy();



//----------------------------------------------------------
// Debug
//----------------------------------------------------------

/*
    Print sensor values to Serial.

    Helpful during development.
*/
void printSensorReport();

#endif
