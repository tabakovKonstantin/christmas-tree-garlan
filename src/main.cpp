#include <Arduino.h>
#include "ConnectManager.h"
#include "FileManager.h"
#include "ConfigManager.h"
#include "MqttManager.h"
#include "LedControl.h"

MqttManager mqttManager;

void setup() {
   Serial.begin(115200);

    if (!FileManager::begin())
    {
        Serial.println("Ошибка инициализации файловой системы!");
        return;
    }

    initWiFi();
    mqttManager.init();
   
    initLEDs();
    uint32_t color = (123 << 16) | (124 << 8) | 23;
    setLEDColor(color);
    
   
}

void loop() {  
}
