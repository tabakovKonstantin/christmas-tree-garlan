// #include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <Arduino.h>
#include "ConnectManager.h"
#include "FileManager.h"
#include "ConfigManager.h"
#include "MqttManager.h"

MqttManager mqttManager;

void setup() {
   Serial.begin(115200);

    if (!FileManager::begin())
    {
        Serial.println("Ошибка инициализации файловой системы!");
        return;
    }

    initWiFi();

     Config config;
     ConfigManager::loadConfig(config);
     Serial.println(config.toJson());

    mqttManager.initMQTT();
   
}

void loop() {
  mqttManager.handleMQTT();
    // put your main code here, to run repeatedly:   
}
