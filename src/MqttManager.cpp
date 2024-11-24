#include "MqttManager.h"
#include "ConfigManager.h"
#include <Ticker.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

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

    mqttClient.subscribe(getCommandTopic().c_str(), 2);

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
    doc["unique_id"] = getProductId();
    doc["command_topic"] = getCommandTopic();

    JsonObject device = doc["device"].to<JsonObject>();
    JsonArray identifiers = device["identifiers"].to<JsonArray>();
    identifiers.add(getProductId());
    device["manufacturer"] = "Tabakov";
    device["model"] = "Home";
    device["name"] = "Christmas garland";
    device["sw_version"] = "0.0.1";

    JsonArray colorModes = doc["supported_color_modes"].to<JsonArray>();
    colorModes.add("rgb");

    doc["effect"] = true;
    JsonArray effectList = doc["effect_list"].to<JsonArray>();
    effectList.add("Rainbow");
    effectList.add("Smooth wave");
    effectList.add("Sparkle");

    doc["schema"] = "json";
    doc["optimistic"] = true;

    String message;
    serializeJson(doc, message);

    mqttClient.publish(getDiscoveryTopic().c_str(), 1, true, message.c_str());
}

String MqttManager::getProductId()
{
    char chipIdStr[11];
    itoa(ESP.getChipId(), chipIdStr, 10);
    return "garland-" + String(chipIdStr);;
}

String MqttManager::getDiscoveryTopic()
{
    String productId = getProductId();

    char discoveryTopic[100];
    snprintf(discoveryTopic, sizeof(discoveryTopic), DISCOVERY_TOPIC_TEMPLATE, productId.c_str());

    return String(discoveryTopic);
}

String MqttManager::getCommandTopic()
{
    String productId = getProductId();

    char commandTopic[100];
    snprintf(commandTopic, sizeof(commandTopic), COMMAND_TOPIC_TEMPLATE, productId.c_str());

    return String(commandTopic);
}
