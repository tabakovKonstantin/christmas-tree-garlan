#include "ConnectManager.h"
#include "ConfigManager.h"
#include <ESP8266WiFi.h>

Config config;

void initWiFi()
{
    Serial.println();
    Serial.println("Initializing WiFi...");

    const char *ssid = "A1_CA2F";
    const char *password = "48575443202C3FAA";

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    
    config.mqttServer = "192.168.100.115";
    config.mqttPort = 1883;

    ConfigManager::saveConfig(config);
}

String getSsidWithChipId()
{
    uint32_t chipId = ESP.getChipId();
    char chipIdStr[11];
    itoa(chipId, chipIdStr, 10);

    String ssid = "Garland-" + String(chipIdStr);

    return ssid;
}