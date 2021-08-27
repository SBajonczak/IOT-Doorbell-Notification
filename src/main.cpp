#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "ConfigurationManager.h"
#include <Ticker.h>

const char *TOPICNAME = "devices/doorbell/active";
WiFiClient espClient;
PubSubClient client(espClient);
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
  if (!client.connected())
  {
    reconnect();
  }
  client.publish(TOPICNAME, "1");
  delay(1000);
  client.publish(TOPICNAME, "0");
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
