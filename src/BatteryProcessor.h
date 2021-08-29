#ifndef BatteryProcessor_H
#define BatteryProcessor_H
#include <ESP8266WiFi.h>


class BatteryProcessor
{
public:
  BatteryProcessor();
  void setup();
  float getVolt();
  bool IsLowWarning();
  bool IsLow();

private:
  float adjust;
  float lastvoltage;
};

#endif