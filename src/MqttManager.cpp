#include "MqttManager.h"
#include "ConfigManager.h"
#include "Payload.h"
// #include "LedControl.h"

const char *DISCOVERY_TOPIC = "homeassistant/light/my_light_1/config";
const char *COMAND_TOPIC = "home/lights/my_light_1/set";

MqttManager::MqttManager() : client(espClient)
{
    
}

void MqttManager::initMQTT()
{
     Serial.println(config.toJson());
     ConfigManager::loadConfig(config);
      Serial.println(config.toJson());
    client.setServer(config.mqttServer.c_str(), config.mqttPort);
    client.setCallback([this](char *topic, byte *payload, unsigned int length)
                       { callback(topic, payload, length); });

    while (!client.connected())
    {
        Serial.println("Попытка подключения к MQTT брокеру...");

        if (client.connect("ESP8266"))
        {
            Serial.println("Подключено к MQTT брокеру.");
            publishDiscoveryMessage();
            client.subscribe(COMAND_TOPIC);
        }  else {

        delay(5000);
        Serial.println(String("Не удалось подключиться. Ошибка: ") + client.state());
        }
    }
}

void MqttManager::callback(char *topic, byte *payload, unsigned int length)
{
    String message = String((char*)payload);  
    Serial.println("Received message: " + message);

    Payload incomingPayload;
    if (incomingPayload.fromJson(message))
    {
        Serial.println("Successfully parsed JSON message.");
        Serial.println(incomingPayload.toJson());
         Serial.println(incomingPayload.brightness);
         Serial.println(incomingPayload.effect);
        // changeState(incomingPayload);
    }
    else
    {
        Serial.println("Failed to parse JSON message.");
    }
}

void MqttManager::handleMQTT()
{
    if (!client.connected())
    {
        initMQTT();
    }
    client.loop();
}

void MqttManager::publish(const char *topic, const char *message)
{
    Serial.println("Publishing to topic: " + String(topic));
    Serial.println("Message: " + String(message));
    client.publish(topic, message);
}

void MqttManager::publishDiscoveryMessage()
{
    JsonDocument doc;
    doc["name"] = "My Light123";
    doc["unique_id"] = "my_light_1";
    // doc["state_topic"] = "home/lights/my_light_1/state";
    doc["command_topic"] = COMAND_TOPIC;
    doc["brightness"] = true;
    doc["rgb"] = true;
    JsonArray colorModes = doc["supported_color_modes"].to<JsonArray>();
    colorModes.add("rgb");
    doc["effect"] = true;
    JsonArray effectList = doc["effect_list"].to<JsonArray>();
    effectList.add("e22");
    effectList.add("e1");
    doc["schema"] = "json";
    doc["optimistic"] = true;

    String message;
    serializeJson(doc, message);

    publish(DISCOVERY_TOPIC, message.c_str());
    //  publish("DISCOVERY_TOPIC1", message.c_str());
    //  publish("DISCOVERY_TOPIC/1/", message.c_str());
    //   publish("homeassistant/1/", message.c_str());
    //  publish("homeassistant/light/my_light_1/config", message.c_str());
}
