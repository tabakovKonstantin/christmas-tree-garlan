#include "ConnectManager.h"
#include "ConfigManager.h"
#include <WiFiManager.h>

WiFiManager wifiManager;
Config config;

void initWiFi()
{
    Serial.println();
    Serial.println("Initializing WiFi...");

    WiFiManagerParameter mqttServer("mqtt_server", "MQTT Broker", "192.168.100.115", 40);
    WiFiManagerParameter mqttPort("mqtt_port", "MQTT Broker port", "1883", 40);
    wifiManager.addParameter(&mqttServer);
    wifiManager.addParameter(&mqttPort);

    if (!wifiManager.autoConnect(getSsidWithChipId().c_str()))
    {
        Serial.println("WiFi подключение не удалось");
        ESP.restart();
    }

    Serial.println("Подключено к WiFi. IP-адрес: " + WiFi.localIP().toString());

    config.mqttServer = mqttServer.getValue();
    config.mqttPort = String(mqttPort.getValue()).toInt();

    ConfigManager::saveConfig(config);
}

String getSsidWithChipId() {
    uint32_t chipId = ESP.getChipId();
    char chipIdStr[11];
    itoa(chipId, chipIdStr, 10);

    String ssid = "Garland-" + String(chipIdStr);

    return ssid;
}