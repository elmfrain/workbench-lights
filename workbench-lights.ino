#include "LightCircuit.hpp"
#include "SystemStatus.hpp"

#ifndef _AVR_IOXXX_H_
#include <avr/iom328p.h>
#endif

LightCircuit mainLED;
LightCircuit auxLED;

SystemStatus sysStatus;

unsigned long lastPrintTime = 0;
unsigned long lastUpdateTime = 0;
bool printingEnabled = true;

void setup()
{
    mainLED.setupVoltageSense(A4, 0.05376);
    mainLED.setupAdjustments(A0, 27.0f, 33.0f);
    mainLED.setupProtection(34.0f, 180);
    mainLED.setupPWM(3);

    auxLED.setupVoltageSense(A5, 4.8876e-3);
    auxLED.setupAdjustments(A1, 2.6f, 3.55f);
    auxLED.setupProtection(3.6f, 129);
    auxLED.setupPWM(11);

    sysStatus.setupSensors(A6, 0.02935f, 4, 7);

    Serial.begin(115200);
}

void loop()
{
    float sysVoltage = sysStatus.getVoltage();

    if(sysVoltage < 11 || 13 < sysVoltage)
    {
        mainLED.setEnabled(false);
        auxLED.setEnabled(false);
    }
    else
    {
        mainLED.setEnabled(true);
        auxLED.setEnabled(true);
    }

    mainLED.updateAdj();
    auxLED.updateAdj();

    unsigned long now = millis();
    if(26 < now - lastUpdateTime)
    {
        mainLED.setEnabled(sysStatus.isSw1On());
        auxLED.setEnabled(sysStatus.isSw2On());

        mainLED.updatePWM();
        auxLED.updatePWM();

        lastUpdateTime = now;
    }
    else if(printingEnabled && 250 < now - lastPrintTime)
    {
        mainLED.printStatus("MainLED");
        auxLED.printStatus("AuxLED");
        sysStatus.printStatus("System");
        Serial.println();
        lastPrintTime = now;
    }
}
