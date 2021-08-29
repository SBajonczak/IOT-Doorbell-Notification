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
String ConfigurationManager::GetMqttUser() { return this->MqttUser; }
String ConfigurationManager::GetMqttPassword() { return this->MqttPassword; }
String ConfigurationManager::GetBaseTopic() { return this->BaseToic; }



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
  doc["wifi"]["ssid"] = this->GetWifiSsid();
  doc["wifi"]["password"] = this->GetWifiPassword();
  doc["mqtt"]["server"] = this->GetMqttServer();
  doc["mqtt"]["port"] = this->GetMqttPort();
  doc["mqtt"]["user"] = this->GetMqttUser();
  doc["mqtt"]["password"] = this->GetMqttPassword();
  doc["mqtt"]["basetopic"] = this->GetBaseTopic();

  
  return doc;
}

void ConfigurationManager::ApplyJsonInput(String json)
{
  DynamicJsonDocument jData(2048);
  deserializeJson(jData, json);


  this->MqttServer = jData["mqtt"]["server"].as<String>();
  this->MqttUser = jData["mqtt"]["user"].as<String>();
  this->MqttPassword = jData["mqtt"]["password"].as<String>();
  this->MqttPort = jData["mqtt"]["port"];
  this->BaseToic = jData["mqtt"]["basetopic"].as<String>();

  this->WifiSsid = jData["wifi"]["ssid"].as<String>();
  this->WifiPassword = jData["wifi"]["password"].as<String>();
  
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
