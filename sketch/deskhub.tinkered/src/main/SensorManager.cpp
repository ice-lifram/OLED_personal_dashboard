#include "SensorManager.h"
#include <math.h>

void SensorManager::init() {
    pinMode(PIN_THERMISTOR, INPUT);
    pinMode(PIN_LDR, INPUT);
    pinMode(PIN_SOUND_SENSOR, INPUT);
    pinMode(PIN_ULTRASONIC_TRIG, OUTPUT);
    pinMode(PIN_ULTRASONIC_ECHO, INPUT);
    
    digitalWrite(PIN_ULTRASONIC_TRIG, LOW);
    
    temperature = 0.0;
    lightLevel = 0;
    soundDetected = false;
    distance = 0;
    lastSensorRead = 0;
}

void SensorManager::update() {
    unsigned long now = millis();
    if (now - lastSensorRead >= SENSOR_READ_INTERVAL) {
        lastSensorRead = now;
        
        readThermistor();
        readLDR();
        readSoundSensor();
        readUltrasonic();
    }
}

void SensorManager::readThermistor() {
    int raw = analogRead(PIN_THERMISTOR);
    
    // Convert ADC to resistance
    float resistance = THERMISTOR_SERIES_RESISTOR / ((1023.0 / raw) - 1.0);
    
    // Steinhart-Hart equation
    float steinhart;
    steinhart = resistance / THERMISTOR_NOMINAL;
    steinhart = log(steinhart);
    steinhart /= THERMISTOR_B_COEFFICIENT;
    steinhart += 1.0 / (THERMISTOR_TEMP_NOMINAL + 273.15);
    steinhart = 1.0 / steinhart;
    steinhart -= 273.15;
    
    temperature = steinhart;
}

void SensorManager::readLDR() {
    lightLevel = analogRead(PIN_LDR);
}

void SensorManager::readSoundSensor() {
    soundDetected = digitalRead(PIN_SOUND_SENSOR) == HIGH;
}

void SensorManager::readUltrasonic() {
    digitalWrite(PIN_ULTRASONIC_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_ULTRASONIC_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_ULTRASONIC_TRIG, LOW);
    
    unsigned long duration = pulseIn(PIN_ULTRASONIC_ECHO, HIGH, ULTRASONIC_TIMEOUT);
    
    if (duration == 0) {
        distance = -1.0;
    } else {
        distance = duration * 0.034 / 2.0;
    }
} 
