#include <ConfigurationManager.h>
#include <EEPROM.h> // Reference for Memmory handling
#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h> // Include the SPIFFS library
#define CONFIG_FILE "/config.json"

// Avoiding compile issues
ConfigurationManager *ConfigurationManager::instance = nullptr;

#define JSON_DOCSIZE 1024

void ConfigurationManager::setup()
{
}

ConfigurationManager::ConfigurationManager()
{
  //this->ReadSettings();
}

String ConfigurationManager::GetWifiSsid() { return this->WifiSsid; }
String ConfigurationManager::GetWifiPassword() { return this->WifiPassword; }
String ConfigurationManager::GetMqttServer() { return this->MqttServer; }
int ConfigurationManager::GetMqttPort() { return this->MqttPort; }
String ConfigurationManager::GetClientName() { return this->ClientName; }

String ConfigurationManager::GetMqttUser() { return this->MqttUser; }
String ConfigurationManager::GetMqttPassword() { return this->MqttPassword; }
String ConfigurationManager::GetBaseTopic() { return this->BaseToic; }
int ConfigurationManager::GetSleepTime() { return this->SleepTime; }
float ConfigurationManager::GetVoltageMultiplicator() { return this->voltagemuliplicator; }
String ConfigurationManager::GetStaticIPAdress() { return this->StaticIPAdress; }
String ConfigurationManager::GetWifiSubnet() { return this->Wifisubnet; }
String ConfigurationManager::GetWifigateway() { return this->Wifigateway; }

String ConfigurationManager::GetJson()
{
  String values;
  DynamicJsonDocument doc = this->GetJsonDocument();
  serializeJson(doc, values);
  return values;
}

bool ConfigurationManager::HasValidConfiguration()
{
  bool exists = false;
  if (SPIFFS.begin())
  {
    exists = SPIFFS.exists(CONFIG_FILE);
  }
  return exists;
}

DynamicJsonDocument ConfigurationManager::GetJsonDocument()
{
  DynamicJsonDocument doc(JSON_DOCSIZE);
  doc["wifi"]["gateway"] = this->GetWifigateway();
  doc["wifi"]["subnet"] = this->GetWifiSubnet();

  doc["wifi"]["ssid"] = this->GetWifiSsid();
  doc["wifi"]["password"] = this->GetWifiPassword();
  doc["wifi"]["staticIp"] = this->GetStaticIPAdress();
  doc["mqtt"]["server"] = this->GetMqttServer();
  doc["mqtt"]["port"] = this->GetMqttPort();
  doc["mqtt"]["user"] = this->GetMqttUser();
  doc["mqtt"]["password"] = this->GetMqttPassword();
  doc["mqtt"]["basetopic"] = this->GetBaseTopic();
  doc["sleep"]["time"] = this->GetSleepTime();

  return doc;
}

void ConfigurationManager::ApplyJsonInput(String json)
{
  DynamicJsonDocument jData(2048);
  deserializeJson(jData, json);

  this->Wifisubnet = jData["wifi"]["subnet"].as<String>();
  this->Wifigateway = jData["wifi"]["gateway"].as<String>();
  this->StaticIPAdress = jData["wifi"]["staticIp"].as<String>();
  this->voltagemuliplicator = jData["battery"]["muliplicator"].as<float>();
  this->MqttServer = jData["mqtt"]["server"].as<String>();
  this->MqttUser = jData["mqtt"]["user"].as<String>();
  this->MqttPassword = jData["mqtt"]["password"].as<String>();
  this->MqttPort = jData["mqtt"]["port"];
  this->BaseToic = jData["mqtt"]["basetopic"].as<String>();
  this->ClientName = jData["mqtt"]["clientname"].as<String>();
  this->WifiSsid = jData["wifi"]["ssid"].as<String>();
  this->WifiPassword = jData["wifi"]["password"].as<String>();
  this->StaticIPAdress = jData["wifi"]["staticIp"].as<String>();
  this->SleepTime = jData["sleep"]["timeinMinutes"].as<int>();
}

void ConfigurationManager::StoreSettings()
{
  String json = this->GetJson();
  if (SPIFFS.begin())
  {
    SPIFFS.remove(CONFIG_FILE);
    File configFile = SPIFFS.open(CONFIG_FILE, "w");
    if (configFile)
    {
      configFile.println(json);
      configFile.close();
    }
  }
}

void ConfigurationManager::ReadSettings()
{

  if (SPIFFS.begin())
  {
    if (SPIFFS.exists(CONFIG_FILE))
    {

      File configFile = SPIFFS.open(CONFIG_FILE, "r");
      if (configFile)
      {
        String s = configFile.readString();
        this->ApplyJsonInput(s);
        return;
      }
    }
  }
}
