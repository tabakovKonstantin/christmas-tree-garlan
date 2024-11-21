#include "MqttManager.h"
#include "ConfigManager.h"
#include <Ticker.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#define PRODUCT_ID "c64b231c003d"
#define DISCOVERY_TOPIC "homeassistant/light/" PRODUCT_ID "/config"
#define COMMAND_TOPIC "home/lights/" PRODUCT_ID "/set"

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

MqttManager::MqttManager(LedControl &led_Control) : ledControl(led_Control)
{
}

void MqttManager::init()
{
    Serial.println();
    Serial.println("Initializing MQTT Manager...");

    mqttClient.onConnect([this](bool sessionPresent)
                         { this->onMqttConnect(sessionPresent); });
    mqttClient.onDisconnect([this](AsyncMqttClientDisconnectReason reason)
                            { this->onMqttDisconnect(reason); });
    mqttClient.onMessage([this](char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
                         { this->onMqttMessage(topic, payload, properties, len, index, total); });

    IPAddress mqttIP;
    ConfigManager::loadConfig(config);
    if (mqttIP.fromString(config.mqttServer.c_str()))
    {
        mqttClient.setServer(mqttIP, config.mqttPort);
    }
    else
    {
        Serial.println("Invalid IP address format.");
        Serial.println(config.mqttServer);
        return;
    }

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
    Serial.println();
    Serial.println("Message received.");
    Serial.print("  topic: ");
    Serial.println(topic);
    Serial.print("  payload: ");

    String message = String((char *)payload);
    Serial.println(message);

    Payload incomingPayload;
    if (incomingPayload.fromJson(message))
    {
        Serial.print("  parsed JSON: ");
        Serial.println(incomingPayload.toJson());
        ledControl.changeState(incomingPayload);
        return;
    }

    Serial.print("Failed to parse JSON message.");
}

void MqttManager::publishDiscoveryMessage()
{
    JsonDocument doc;
    doc["name"] = "Christmas garland";
    doc["unique_id"] = PRODUCT_ID;
    doc["command_topic"] = COMMAND_TOPIC;

    JsonObject device = doc.createNestedObject("device");
    JsonArray identifiers = device.createNestedArray("identifiers");
    identifiers.add(PRODUCT_ID);
    device["manufacturer"] = "Tabakov";
    device["model"] = "Home";
    device["name"] = "Christmas garland";
    device["sw_version"] = "0.0.1";

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
