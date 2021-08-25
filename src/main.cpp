#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "ConfigurationManager.h"
#include <Ticker.h>
Ticker ticker;

#define NB_TRYWIFI 10

WiFiClient espClient;
PubSubClient client(espClient);

void tick()
{
  //toggle state
  int state = digitalRead(BUILTIN_LED); // get the current state of GPIO1 pin
  digitalWrite(BUILTIN_LED, !state);    // set pin to the opposite state
}


boolean reconnect()
{
  if (client.connect("ESP8266Client", 
  ConfigurationManager::getInstance()->GetMqttUser().c_str(), 
  ConfigurationManager::getInstance()->GetMqttPassword().c_str()
  )) {
    Serial.println("Connected to server");
    return client.connected();
  }
  Serial.println("I think connection failed!");
  return 0;
}

void sendMQTTMessage()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.publish("doorbell/active", "1");
  delay(1000);
  client.publish("doorbell/active", "0");
}

void setup()
{
  int start = millis();
  pinMode(BUILTIN_LED, OUTPUT);
  ticker.attach(0.5, tick);

  Serial.begin(115200);
  Serial.println("");
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
    if (_try >= NB_TRYWIFI)
    {
      Serial.println("Impossible to connect WiFi network, go to deep sleep");
      ESP.deepSleep(0);
    }
  }
  Serial.println("Connected to the WiFi network");

  Serial.println("Send PIR Status to MQTT broker");
  client.setServer(ConfigurationManager::getInstance()->GetMqttServer().c_str(), ConfigurationManager::getInstance()->GetMqttPort());
  sendMQTTMessage();
  Serial.println("Waking time: ");
  Serial.print(millis() - start);
  Serial.println("ms");

  ticker.detach();
  digitalWrite(BUILTIN_LED, LOW);

  Serial.println("Go to deep sleep");

  ESP.deepSleep(0);
}

void loop()
{
}
