#ifndef SYSTEM_STATUS_HPP
#define SYSTEM_STATUS_HPP

class SystemStatus
{
public:
    SystemStatus();

    void setupSensors(int voltageSensePin, float voltageInputFactor, int sw1Pin, int sw2Pin);

    float getVoltage();

    bool isSw1On();
    bool isSw2On();

    void printStatus(const char* name);
private:
    int voltageSensePin_m;
    float voltageInputFactor_m;

    int sw1Pin_m;
    int sw2Pin_m;
};

#endif //SYSTEM_STATUS_HPP