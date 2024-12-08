#include <Arduino.h>
#include "ConnectManager.h"
#include "FileManager.h"
#include "ConfigManager.h"
#include "MqttManager.h"
#include "LedControl.h"
#include <ESP8266WiFi.h>
#define FLASH_BUTTON_PIN 0 

EffectManager effectManager;
LedControl ledControl(effectManager);
MqttManager mqttManager(ledControl);

void setup()
{
  Serial.begin(115200);

  if (!FileManager::begin())
  {
    Serial.println("Ошибка инициализации файловой системы!");
    return;
  }

  ledControl.initLEDs();

  Serial.println("!!!!");
  initWiFi();

  if (WiFi.getMode() == WIFI_STA)
  {
    Serial.println("Currently in Station mode (STA)");
    mqttManager.init();
  }
}

void loop()
{
  FastLED.show();
  
  // bool reading = digitalRead(FLASH_BUTTON_PIN) == LOW;
  // if (reading) {
  //  ConfigManager::eraseConfig();
  // }

}
