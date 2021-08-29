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
