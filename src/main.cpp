#include <Arduino.h>
#include "ConnectManager.h"
#include "FileManager.h"
#include "ConfigManager.h"
#include "MqttManager.h"
#include "LedControl.h"
#include <FastLED.h>

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
}

void loop() {  
    FastLED.show();
}
