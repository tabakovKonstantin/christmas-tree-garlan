#ifndef MY_CONFIG_H
#define MY_CONFIG_H

#include <ArduinoJson.h>

class Config {
public:
    String mqttServer;
    int mqttPort;

    String toJson() const {
        JsonDocument jsonDoc; 
        jsonDoc["mqttServer"] = mqttServer;
        jsonDoc["mqttPort"] = mqttPort;

        String jsonString;
        serializeJson(jsonDoc, jsonString);
        return jsonString;
    }

    // Метод для заполнения объекта из JSON строки
    bool fromJson(const String& jsonString) {
        JsonDocument jsonDoc;
        DeserializationError error = deserializeJson(jsonDoc, jsonString);
        if (error) {
            Serial.println("Ошибка парсинга JSON: " + String(error.c_str()));
            return false;
        }

        mqttServer = jsonDoc["mqttServer"].as<String>();
        mqttPort = jsonDoc["mqttPort"].as<int>();
        return true;
    }

};

#endif // MY_CONFIG_H