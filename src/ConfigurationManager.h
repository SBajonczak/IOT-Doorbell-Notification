#include<Arduino.h>
#include <ArduinoJson.h>
/*
  StateManager.h - Helper Libary for managing the Homie states.
*/
#ifndef ConfigurationManager_h
#define ConfigurationManager_h

class ConfigurationManager
{

public:
  ConfigurationManager();
  void setup();
  bool HasValidConfiguration();

  String GetWifiSsid();
  String GetWifiPassword();
  String GetMqttServer();
  int GetMqttPort();
  String GetMqttUser();
  String GetMqttPassword();
  String GetBaseTopic();

  String GetJson();
  void StoreSettings();
  void ReadSettings();
  void ApplyJsonInput(String json);
  // Get an Singelton instance
  static ConfigurationManager *getInstance()
  {
    if (instance == nullptr)
    {
      instance = new ConfigurationManager();
    }
    return instance;
  }

private:
  static ConfigurationManager *instance;
  float VccAdjustSetting;
  int SleepTime;
  String WifiSsid;
  String WifiPassword;
  String BaseToic;

  String MqttServer;
  String MqttUser;
  int MqttPort;
  String MqttPassword;

  byte CurrentState;

  DynamicJsonDocument GetJsonDocument();
};
#endif