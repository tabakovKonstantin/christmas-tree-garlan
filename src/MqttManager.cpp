#include "MqttManager.h"
#include "ConfigManager.h"
#include "LedControl.h"
#include <Ticker.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#define DISCOVERY_TOPIC "homeassistant/light/my_light_1/config"
#define COMMAND_TOPIC "home/lights/my_light_1/set"
#define MQTT_HOST IPAddress(192, 168, 100, 115)
#define MQTT_PORT 1883

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;
Ticker wifiReconnectTimer;

MqttManager::MqttManager()
{
    ConfigManager::loadConfig(config);
}

void MqttManager::init()
{
    Serial.println("Initializing MQTT Manager...");

    mqttClient.onConnect([this](bool sessionPresent)
                         { this->onMqttConnect(sessionPresent); });
    mqttClient.onDisconnect([this](AsyncMqttClientDisconnectReason reason)
                            { this->onMqttDisconnect(reason); });
    mqttClient.onMessage([this](char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
                         { this->onMqttMessage(topic, payload, properties, len, index, total); });

    mqttClient.setServer(MQTT_HOST, MQTT_PORT);

    connectToMqtt();
}

void MqttManager::connectToMqtt()
{
    Serial.println("Connecting to MQTT...");
    mqttClient.connect();
}

void MqttManager::onMqttConnect(bool sessionPresent)
{
    Serial.println("Connected to MQTT.");
    Serial.print("Session present: ");
    Serial.println(sessionPresent);

    mqttClient.subscribe(COMMAND_TOPIC, 2);

    publishDiscoveryMessage();
}

void MqttManager::onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
    Serial.println("Disconnected from MQTT.");
    if (WiFi.isConnected())
    {
        mqttReconnectTimer.once(2, [this]()
                                { this->connectToMqtt(); });
    }
}

void MqttManager::onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
    Serial.println("Message received.");
    Serial.print("  topic: ");
    Serial.println(topic);
    Serial.print("  payload: ");

    String message = String((char *)payload);
    Serial.println(message);

    Payload incomingPayload;
    if (incomingPayload.fromJson(message))
    {
        Serial.println("  parsed JSON: ");
        Serial.print(incomingPayload.toJson());
        changeState(incomingPayload);
        return;
    }

    Serial.println("Failed to parse JSON message.");
}

void MqttManager::publishDiscoveryMessage()
{
    JsonDocument doc;
    doc["name"] = "My Light";
    doc["unique_id"] = "my_light_1";
    doc["command_topic"] = COMMAND_TOPIC;
    doc["brightness"] = true;
    doc["rgb"] = true;

    JsonArray colorModes = doc.createNestedArray("supported_color_modes");
    colorModes.add("rgb");

    doc["effect"] = true;
    JsonArray effectList = doc.createNestedArray("effect_list");
    effectList.add("effect1");
    effectList.add("effect2");

    doc["schema"] = "json";
    doc["optimistic"] = true;

    String message;
    serializeJson(doc, message);

    mqttClient.publish(DISCOVERY_TOPIC, 1, true, message.c_str());
}
