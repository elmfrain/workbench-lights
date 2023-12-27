#include "SystemStatus.hpp"

#include <Arduino.h>

SystemStatus::SystemStatus() :
    voltageSensePin_m(255),
    voltageInputFactor_m(0.0f),
    sw1Pin_m(255),
    sw2Pin_m(255)
{
}

void SystemStatus::setupSensors(int voltageSensePin, float voltageInputFactor, int sw1Pin, int sw2Pin)
{
    voltageSensePin_m = voltageSensePin;
    voltageInputFactor_m = voltageInputFactor;
    sw1Pin_m = sw1Pin;
    sw2Pin_m = sw2Pin;

    pinMode(sw1Pin_m, INPUT_PULLUP);
    pinMode(sw2Pin_m, INPUT_PULLUP);
}

float SystemStatus::getVoltage()
{
    int reading = analogRead(voltageSensePin_m);
    reading = analogRead(voltageSensePin_m);

    return reading * voltageInputFactor_m;
}

bool SystemStatus::isSw1On()
{
    return !digitalRead(sw1Pin_m);
}

bool SystemStatus::isSw2On()
{
    return !digitalRead(sw2Pin_m);
}

void SystemStatus::printStatus(const char* name)
{
    char output[100];
    char sVoltage[7];

    dtostrf(getVoltage(), 4, 2, sVoltage);

    sprintf(output, "[%s] Voltage: %sV Switch1: %d Switch2 %d",
            name, sVoltage, isSw1On(), isSw2On());

    Serial.println(output);
}