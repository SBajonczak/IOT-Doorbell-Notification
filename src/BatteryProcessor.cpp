#include "BatteryProcessor.h"

float adjust;

BatteryProcessor::BatteryProcessor()
{
    pinMode(A0,INPUT);
}

float BatteryProcessor::getVolt()
{
    this->lastvoltage = analogRead(A0);
    return this->lastvoltage;
}

bool BatteryProcessor::IsLowWarning()
{
    if (this->lastvoltage < 0)
        this->getVolt();
    return this->lastvoltage < BATT_WARNING_VOLTAGE;
}
bool BatteryProcessor::IsLow()
{
    if (this->lastvoltage < 0)
        this->getVolt();
    return this->lastvoltage < BATT_LOW_VOLTAGE;
}