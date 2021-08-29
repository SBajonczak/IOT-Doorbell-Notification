#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>

#include "ConfigurationManager.h"
#include "BatteryProcessor.h"

const char *BELL_TOPIC_NAME = "devices/doorbell/active";
const char *BATTERY_TOPIC_NAME = "devices/doorbell/battery";
WiFiClient espClient;
PubSubClient client(espClient);
BatteryProcessor battery;
#ifdef USE_LED
Ticker ticker;
#endif

#ifdef USE_LED
void tick()
{

  //toggle state
  int state = digitalRead(BUILTIN_LED); // get the current state of GPIO1 pin
  digitalWrite(BUILTIN_LED, !state);    // set pin to the opposite state
}
#endif

boolean reconnect()
{
  if (client.connect("Doorbell",
                     ConfigurationManager::getInstance()->GetMqttUser().c_str(),
                     ConfigurationManager::getInstance()->GetMqttPassword().c_str()))
  {
    Serial.println("Connected to server");
    return client.connected();
  }
  Serial.println("There is no connection!");
  return 0;
}

void sendMQTTMessage()
{

  char deviceId[13]; // need to define the static variable
  uint8_t mac[6];
  WiFi.macAddress(mac);
  snprintf(deviceId, 13, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  if (!client.connected())
  {
    reconnect();
  }
  String bellTopic= ConfigurationManager::getInstance()->GetBaseTopic()+"/"+ deviceId + "/active";
  String batTopic = ConfigurationManager::getInstance()->GetBaseTopic()+"/"+ deviceId + "/voltage";
  client.publish(bellTopic.c_str(), "1");
  client.publish(batTopic.c_str(), String(battery.getVolt()).c_str());
  delay(1000);
}

void setup()
{
  int start = millis();

#ifdef USE_LED
  pinMode(BUILTIN_LED, OUTPUT);
  ticker.attach(0.5, tick);
#endif

  Serial.begin(115200);
  Serial.print("Reason startup :");
  Serial.println(ESP.getResetReason());

  ConfigurationManager::getInstance()->ReadSettings();
  Serial.println(ConfigurationManager::getInstance()->GetWifiSsid());
  WiFi.begin(ConfigurationManager::getInstance()->GetWifiSsid(), ConfigurationManager::getInstance()->GetWifiPassword());
  Serial.println("Connecting to WiFi.");
  int _try = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
    _try++;
    if (_try >= WIFI_CONNECT_TRY_COUNTER)
    {
      Serial.println("Impossible to connect WiFi network, go to deep sleep");
      ESP.deepSleep(0);
    }
  }
  Serial.println("Connected to the WiFi network");
  IPAddress ipAdress;
  ipAdress.fromString(ConfigurationManager::getInstance()->GetMqttServer());
  client.setServer(ipAdress, ConfigurationManager::getInstance()->GetMqttPort());

  sendMQTTMessage();

#ifdef USE_LED
  ticker.detach();
  digitalWrite(BUILTIN_LED, LOW);
#endif

  Serial.println("Go to deep sleep");

  ESP.deepSleep(0);
}

void loop()
{
}
