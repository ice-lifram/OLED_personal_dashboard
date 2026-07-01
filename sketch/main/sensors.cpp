/*
==========================================================
 sensors.cpp
----------------------------------------------------------
 Sensor Module

 Responsibilities
    - Initialize sensors
    - Read sensor values
    - Validate readings
    - Store latest measurements
    - Provide processed data

 NOTE:
    This file intentionally serves as a framework.
==========================================================
*/

#include "sensors.h"
#include "config.h"

#include <DHT.h>



//----------------------------------------------------------
// Private Objects
//----------------------------------------------------------

DHT dht(
    DHT_PIN,
    DHT_TYPE
);



//----------------------------------------------------------
// Private Data Storage
//----------------------------------------------------------

SensorData sensorData =
{
    // Environmental
    0.0f,
    0.0f,
    0.0f,

    // Future Sensors
    0.0f,
    0.0f,
    0.0f,
    0.0f,

    // Status Flags
    false,
    false,
    false
};



//----------------------------------------------------------
// Initialization
//----------------------------------------------------------

void initSensors()
{
    dht.begin();

    sensorData.dhtAvailable = true;

    Serial.println(F("Sensors initialized."));
}



//----------------------------------------------------------
// Read All Sensors
//----------------------------------------------------------

void readAllSensors()
{
    readDHT();

    // Future expansion
    // readMQ135();
    // readLDR();
    // readBMP280();
    // readRTC();
}



//----------------------------------------------------------
// DHT
//----------------------------------------------------------

void readDHT()
{
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature))
    {
        sensorData.dhtAvailable = false;

        Serial.println(F("Failed to read DHT sensor."));

        return;
    }

    sensorData.dhtAvailable = true;

    sensorData.temperature = temperature;

    sensorData.humidity = humidity;

    sensorData.heatIndex =
        dht.computeHeatIndex(
            temperature,
            humidity,
            false
        );
}



//----------------------------------------------------------
// Future Sensors
//----------------------------------------------------------

void readMQ135()
{
    /*
        TODO

        Read MQ135
        Calculate air quality
    */
}



void readLDR()
{
    /*
        TODO

        Read ambient light
    */
}



void readBMP280()
{
    /*
        TODO

        Read pressure
        Read altitude
    */
}



void readRTC()
{
    /*
        TODO

        Read current date/time
    */
}



//----------------------------------------------------------
// Data Access
//----------------------------------------------------------

SensorData getSensorData()
{
    return sensorData;
}



//----------------------------------------------------------
// Convenience Getters
//----------------------------------------------------------

float getTemperature()
{
    return sensorData.temperature;
}



float getHumidity()
{
    return sensorData.humidity;
}



float getHeatIndex()
{
    return sensorData.heatIndex;
}



float getAirQuality()
{
    return sensorData.airQuality;
}



float getLightLevel()
{
    return sensorData.lightLevel;
}



float getPressure()
{
    return sensorData.pressure;
}



float getAltitude()
{
    return sensorData.altitude;
}



//----------------------------------------------------------
// Status
//----------------------------------------------------------

bool isDHTAvailable()
{
    return sensorData.dhtAvailable;
}



bool isMQ135Available()
{
    return sensorData.mq135Available;
}



bool isRTCAvailable()
{
    return sensorData.rtcAvailable;
}



//----------------------------------------------------------
// Validation
//----------------------------------------------------------

bool sensorsHealthy()
{
    return sensorData.dhtAvailable;

    /*
        Future:

        return
            sensorData.dhtAvailable &&
            sensorData.mq135Available &&
            sensorData.rtcAvailable;
    */
}



//----------------------------------------------------------
// Debug
//----------------------------------------------------------

void printSensorReport()
{
    Serial.println();

    Serial.println(F("===== SENSOR REPORT ====="));

    Serial.print(F("Temperature : "));
    Serial.print(sensorData.temperature);
    Serial.println(F(" °C"));

    Serial.print(F("Humidity    : "));
    Serial.print(sensorData.humidity);
    Serial.println(F(" %"));

    Serial.print(F("Heat Index  : "));
    Serial.print(sensorData.heatIndex);
    Serial.println(F(" °C"));

    Serial.print(F("DHT Status  : "));
    Serial.println(
        sensorData.dhtAvailable ?
        F("OK") :
        F("FAILED")
    );

    Serial.println(F("========================="));
}
