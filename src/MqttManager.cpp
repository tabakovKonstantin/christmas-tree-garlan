#include "MqttManager.h"
#include "ConfigManager.h"
#include "LedControl.h"
//#include <AsyncMqttClient.h>
#include <Ticker.h>
#include <ArduinoJson.h>

#define DISCOVERY_TOPIC "homeassistant/light/my_light_1/config"
#define COMMAND_TOPIC "home/lights/my_light_1/set"
#define MQTT_HOST IPAddress(192, 168, 100, 115)
#define MQTT_PORT 1883


AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;
Ticker wifiReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

// Конструктор
MqttManager::MqttManager() {
     ConfigManager::loadConfig(config);
}

// Метод для инициализации подключения
void MqttManager::init() {
    Serial.println("Initializing MQTT Manager...");

    // wifiConnectHandler = WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP &event) {
    //     this->onWifiConnect(event);
    // });
    // wifiDisconnectHandler = WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected &event) {
    //     this->onWifiDisconnect(event);
    // });

    

    mqttClient.onConnect([this](bool sessionPresent) { this->onMqttConnect(sessionPresent); });
    mqttClient.onDisconnect([this](AsyncMqttClientDisconnectReason reason) { this->onMqttDisconnect(reason); });
    mqttClient.onSubscribe([this](uint16_t packetId, uint8_t qos) { this->onMqttSubscribe(packetId, qos); });
    mqttClient.onUnsubscribe([this](uint16_t packetId) { this->onMqttUnsubscribe(packetId); });
    mqttClient.onMessage([this](char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
        this->onMqttMessage(topic, payload, properties, len, index, total);
    });
    mqttClient.onPublish([this](uint16_t packetId) { this->onMqttPublish(packetId); });

    mqttClient.setServer(MQTT_HOST, MQTT_PORT);

   mqttClient.connect();
}


// Подключение к MQTT
void MqttManager::connectToMqtt() {
    Serial.println("Connecting to MQTT...");
    mqttClient.connect();
}

// Обработчик подключения к Wi-Fi
void MqttManager::onWifiConnect(const WiFiEventStationModeGotIP &event) {
    Serial.println("Connected to Wi-Fi.");
    connectToMqtt();
}

// Обработчик отключения от Wi-Fi
void MqttManager::onWifiDisconnect(const WiFiEventStationModeDisconnected &event) {
    Serial.println("Disconnected from Wi-Fi.");
    mqttReconnectTimer.detach();
    //wifiReconnectTimer.once(2, [this]() { this->connectToWifi(); });
}

// Обработчик подключения к MQTT
void MqttManager::onMqttConnect(bool sessionPresent) {
    Serial.println("Connected to MQTT.");
    Serial.print("Session present: ");
    Serial.println(sessionPresent);

    // Подписываемся на команды
    mqttClient.subscribe(COMMAND_TOPIC, 2);

    // Публикуем сообщение для автоматической конфигурации Home Assistant
    publishDiscoveryMessage();
}

// Обработчик отключения от MQTT
void MqttManager::onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
    Serial.println("Disconnected from MQTT.");
    if (WiFi.isConnected()) {
        mqttReconnectTimer.once(2, [this]() { this->connectToMqtt(); });
    }
}

// Обработчик подписки
void MqttManager::onMqttSubscribe(uint16_t packetId, uint8_t qos) {
    Serial.println("Subscribe acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
    Serial.print("  qos: ");
    Serial.println(qos);
}

// Обработчик отписки
void MqttManager::onMqttUnsubscribe(uint16_t packetId) {
    Serial.println("Unsubscribe acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
}

// Обработчик входящих сообщений
void MqttManager::onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
    Serial.println("Publish received.");
    Serial.print("  topic: ");
    Serial.println(topic);
    Serial.print("  payload: ");
    // for (size_t i = 0; i < len; i++) {
    //     Serial.print(payload[i]);
    // }
     String message = String((char*)payload);  
    Serial.println("Received message: " + message);

    Payload incomingPayload;
    if (incomingPayload.fromJson(message))
    {
        Serial.println("Successfully parsed JSON message.");
        Serial.println(incomingPayload.toJson());
         Serial.println(incomingPayload.brightness);
         Serial.println(incomingPayload.effect);
        changeState(incomingPayload);
    }
    else
    {
        Serial.println("Failed to parse JSON message.");
    }
    // changeState(incomingPayload);
    Serial.println();
}

// Обработчик публикации
void MqttManager::onMqttPublish(uint16_t packetId) {
    Serial.println("Publish acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
}

// Публикация сообщения конфигурации для Home Assistant
void MqttManager::publishDiscoveryMessage() {
    StaticJsonDocument<512> doc;
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
