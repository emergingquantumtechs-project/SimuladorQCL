#ifndef INPUT_HPP
#define INPUT_HPP

class Input
{
private:

    double appliedVoltage;
    double temperature;
    int numberOfPeriods;

public:

    Input();

    void load();

    double getAppliedVoltage() const;

    double getTemperature() const;

    int getNumberOfPeriods() const;
};

#endif