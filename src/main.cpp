#include <Arduino.h>
#include "ConnectManager.h"
#include "FileManager.h"
#include "ConfigManager.h"
#include "MqttManager.h"
#include "LedControl.h"


EffectManager effectManager;
LedControl ledControl(effectManager);
MqttManager mqttManager(ledControl);

void setup() {
   Serial.begin(115200);

    if (!FileManager::begin())
    {
        Serial.println("Ошибка инициализации файловой системы!");
        return;
    }

    initWiFi();
    ledControl.initLEDs();
    mqttManager.init();
    
}

void loop() {  
   FastLED.show();
}
