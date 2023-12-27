#include "LightCircuit.hpp"

#include <Arduino.h>

#ifndef _AVR_IOXXX_H_
#include <avr/iom328p.h>
#endif

LightCircuit::LightCircuit() :
    voltage_m(0.0f),
    brightness_m(0.0f),
    targetVoltage_m(0.0f),
    minVoltage_m(0.0f),
    maxVoltage_m(0.0f),
    pwmDutyCycle_m(0),
    voltageInputFactor_m(0.0f),
    voltageSensePin_m(255),
    adjustSensePin_m(255),
    pwmPin_m(255),
    protectionMaxVoltage_m(0.0f),
    protectionMaxDutyCycle_m(0),
    enabled_m(false)
{
}

void LightCircuit::setupVoltageSense(int voltageSensePin, float voltageInputFactor)
{
    voltageSensePin_m = voltageSensePin;
    voltageInputFactor_m = voltageInputFactor;
}

void LightCircuit::setupAdjustments(int adjustSensePin, float minVoltage, float maxVoltage)
{
    adjustSensePin_m = adjustSensePin;
    minVoltage_m = minVoltage;
    maxVoltage_m = maxVoltage;
}

void LightCircuit::setupProtection(float maxVoltage, int maxDutyCycle)
{
    protectionMaxVoltage_m = maxVoltage;
    protectionMaxDutyCycle_m = maxDutyCycle;
}

void LightCircuit::setupPWM(int pwmPin)
{
    pwmPin_m = pwmPin;
    pinMode(pwmPin_m, OUTPUT);

    TCCR2B = TCCR2B & B11111000 | B00000001; // set PWM frequency of 31372.55 Hz for pins D3 and D11
}

void LightCircuit::updatePWM()
{
    int reading = analogRead(voltageSensePin_m);
    reading = analogRead(voltageSensePin_m);
    voltage_m = reading * voltageInputFactor_m;

    if(!enabled_m)
    {
        pwmDutyCycle_m = 0;
        goto setPWM;
    }

    if(protectionMaxVoltage_m < voltage_m && 0 < pwmDutyCycle_m)
    {
        pwmDutyCycle_m--;
        goto setPWM;
    }

    if(voltage_m < targetVoltage_m && pwmDutyCycle_m < protectionMaxDutyCycle_m)
    {
        pwmDutyCycle_m++;
    }
    else if(voltage_m > targetVoltage_m && 0 < pwmDutyCycle_m)
    {
        pwmDutyCycle_m--;
    }

setPWM:
    analogWrite(pwmPin_m, pwmDutyCycle_m);
}

void LightCircuit::updateAdj()
{
    int reading = analogRead(adjustSensePin_m);
    reading = analogRead(adjustSensePin_m);

    brightness_m = reading * 9.7752e-4;

    targetVoltage_m = minVoltage_m + (maxVoltage_m - minVoltage_m) * brightness_m;
}

void LightCircuit::setEnabled(bool enabled)
{
    enabled_m = enabled;
}

void LightCircuit::printStatus(const char* name)
{
    char output[100];
    char sVoltage[7];
    char sBrightness[7];

    dtostrf(voltage_m, 4, 2, sVoltage);
    dtostrf(brightness_m, 4, 2, sBrightness);

    sprintf(output, "[%s] Voltage: %sV Brightness: %s PWM: %d/255",
    name, sVoltage, sBrightness, pwmDutyCycle_m);

    Serial.println(output);
}