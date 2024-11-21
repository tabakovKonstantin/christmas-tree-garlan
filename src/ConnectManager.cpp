#include "ConnectManager.h"
#include "ConfigManager.h"
// #include "Config.h"
#include <WiFiManager.h>

WiFiManager wifiManager;
Config config;

void initWiFi()
{
    // WiFi.mode(WIFI_STA);
    wifiManager.resetSettings();
    Serial.println("111");
    WiFiManagerParameter mqttServer("mqtt_server", "MQTT Broker", "mqtt://192.168.100.115", 40);
    WiFiManagerParameter mqttPort("mqtt_port", "MQTT Broker port", "1883", 40);
    wifiManager.addParameter(&mqttServer);
    wifiManager.addParameter(&mqttPort);

    Serial.println("112");
    // ESP.wdtDisable(); 

    if (!wifiManager.autoConnect())
    {
        Serial.println("WiFi подключение не удалось");
     //   ESP.restart();
    }

    Serial.println("Подключено к WiFi. IP-адрес: " + WiFi.localIP().toString());

    config.mqttServer = mqttServer.getValue();
    config.mqttPort = String(mqttPort.getValue()).toInt();

    Serial.println(config.toJson());

    ConfigManager::saveConfig(config);
}
