#include "WifiManager.h"

WifiManager::WifiManager()
{
}

void WifiManager::Initialize()
{
  this->rtcValid = false;
  this->staticAdress.fromString(ConfigurationManager::getInstance()->GetStaticIPAdress());
  this->gateway.fromString(ConfigurationManager::getInstance()->GetWifigateway());
  this->subnet.fromString(ConfigurationManager::getInstance()->GetWifiSubnet());
  ReadRtc();
  this->SetMacAdress();
}

void WifiManager::SetMacAdress()
{
  char deviceId[13]; // need to define the static variable
  uint8_t mac[6];
  WiFi.macAddress(mac);
  snprintf(deviceId, 13, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void WifiManager::ReadRtc()
{
  Serial.println("Try to read from RTC");
  bool rtcValid = false;
  if (ESP.rtcUserMemoryRead(0, (uint32_t *)&rtcData, sizeof(rtcData)))
  {
    // Calculate the CRC of what we just read from RTC memory, but skip the first 4 bytes as that's the checksum itself.
    uint32_t crc = this->calculateCRC32(((uint8_t *)&rtcData) + 4, sizeof(rtcData) - 4);
    if (crc == rtcData.crc32)
    {
      Serial.println("Read from RTC: Sucess");

      this->rtcValid = true;
    }
    else
    {
      Serial.println("Read from RTC: Failure");
    }
  }
}

void WifiManager::Connect()
{
  // Google one
  IPAddress dns(8, 8, 8, 8);
  WiFi.config(this->staticAdress, gateway, subnet, dns);
  WiFi.hostname(ConfigurationManager::getInstance()-> GetClientName());
  if (rtcValid)
  {
    Serial.println("Try Quick Connect");
    // The RTC data was good, make a quick connection
    WiFi.begin(ConfigurationManager::getInstance()->GetWifiSsid(), ConfigurationManager::getInstance()->GetWifiPassword(),
     rtcData.channel, rtcData.bssid, true);
  }
  else
  {
    Serial.println("Try NON Quick Connect");
    // The RTC data was not valid, so make a regular connection
    WiFi.begin(ConfigurationManager::getInstance()->GetWifiSsid(), ConfigurationManager::getInstance()->GetWifiPassword());
  }

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

  this->WriteRtcSettings();
}

void WifiManager::WriteRtcSettings()
{
  rtcData.channel = WiFi.channel();
  // Copy 6 bytes of BSSID (AP's MAC address)
  memcpy(rtcData.bssid, WiFi.BSSID(), 6);
  // Generate crc32
  rtcData.crc32 = this->calculateCRC32(((uint8_t *)&rtcData) + 4, sizeof(rtcData) - 4);
  ESP.rtcUserMemoryWrite(0, (uint32_t *)&rtcData, sizeof(rtcData));
  Serial.println("Write data to RTC");
}

uint32_t WifiManager::calculateCRC32(const uint8_t *data, size_t length)
{
  uint32_t crc = 0xffffffff;
  while (length--)
  {
    uint8_t c = *data++;
    for (uint32_t i = 0x80; i > 0; i >>= 1)
    {
      bool bit = crc & 0x80000000;
      if (c & i)
      {
        bit = !bit;
      }

      crc <<= 1;
      if (bit)
      {
        crc ^= 0x04c11db7;
      }
    }
  }

  return crc;
}