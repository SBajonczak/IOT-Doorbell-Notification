#ifndef WifiManager_h
#define WifiManager_h
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "ConfigurationManager.h"
/*
  StateManager.h - Helper Libary for managing the Homie states.
*/

class WifiManager
{

    // The ESP8266 RTC memory is arranged into blocks of 4 bytes. The access methods read and write 4 bytes at a time,
    // so the RTC data structure should be padded to a 4-byte multiple.
    struct
    {
        uint32_t crc32;   // 4 bytes
        uint8_t channel;  // 1 byte,   5 in total
        uint8_t bssid[6]; // 6 bytes, 11 in total
        uint8_t padding;  // 1 byte,  12 in total
    } rtcData;

public:
    WifiManager();
    void Initialize();
    void Connect();

    static WifiManager *getInstance()
    {
        if (instance == nullptr)
        {
            instance = new WifiManager();
        }
        return instance;
    }

private:
    static WifiManager *instance;
    uint32_t calculateCRC32(const uint8_t *data, size_t length);
    void WriteRtcSettings();
    void SetMacAdress();
    void ReadRtc();
    IPAddress staticAdress;
    IPAddress gateway;
    IPAddress subnet;
    bool rtcValid;
};
#endif