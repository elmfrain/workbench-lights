#ifndef LIGHT_CIRCUIT_HPP
#define LIGHT_CIRCUIT_HPP

class LightCircuit
{
public:
    LightCircuit();

    void setupVoltageSense(int voltageSensePin, float voltageInputFactor);
    void setupAdjustments(int adjustSensePin, float minVoltage, float maxVoltage);
    void setupProtection(float maxVoltage, int maxDutyCycle);
    void setupPWM(int pwmPin, float voltageTolerance);
    void setupPID(float porportional, float integral, float derivative, float maxAccumulation = 20.0f);

    void updatePWM();
    void updateAdj();

    void setEnabled(bool enabled);
    void printStatus(const char* name);

private:
    struct PID
    {
        float p, i, d;
        float error, errorAccumulation, errorRate, maxAccumulation;
        float value;

        PID();

        void newError(float error);
        float control(float min, float max);
    } controller_m;

    float voltage_m;
    float brightness_m;

    float targetVoltage_m;
    float minVoltage_m;
    float maxVoltage_m;
    float voltageTolerance_m;

    int pwmDutyCycle_m;

    float voltageInputFactor_m;
    int voltageSensePin_m;
    int adjustSensePin_m;
    int pwmPin_m;
    
    float protectionMaxVoltage_m;
    int protectionMaxDutyCycle_m;

    bool enabled_m;
};

#endif //LIGHT_CIRCUIT_HPP