#include <Arduino.h>
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

  String GetWifigateway();
  String GetWifiSubnet();
  String GetWifiSsid();
  String GetWifiPassword();
  String GetMqttServer();
  String GetStaticIPAdress();
  int GetMqttPort();
  String GetMqttUser();
  String GetMqttPassword();
  String GetBaseTopic();
  String GetClientName();
  String GetJson();
  int GetSleepTime();
  float GetVoltageMultiplicator();
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
  float voltagemuliplicator;
  String Wifigateway;
  String Wifisubnet;
  String WifiSsid;
  String WifiPassword;
  String StaticIPAdress;
  String BaseToic;
  String ClientName;
  String MqttServer;
  String MqttUser;
  int MqttPort;
  String MqttPassword;

  byte CurrentState;

  DynamicJsonDocument GetJsonDocument();
};
#endif