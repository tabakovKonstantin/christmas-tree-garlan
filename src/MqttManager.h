#ifndef MqttManager_h
#define MqttManager_h

#include "Config.h"
#include <Arduino.h>
#include <AsyncMqttClient.h>
#include <Ticker.h>
#include <ESP8266WiFi.h>

class MqttManager {
public:
    MqttManager();
    void init();
    void connectToMqtt();

private:
    Config config;
    //void connectToMqtt();

    void onWifiConnect(const WiFiEventStationModeGotIP &event);
    void onWifiDisconnect(const WiFiEventStationModeDisconnected &event);

    void onMqttConnect(bool sessionPresent);
    void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);

    void onMqttSubscribe(uint16_t packetId, uint8_t qos);
    void onMqttUnsubscribe(uint16_t packetId);
    void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
    void onMqttPublish(uint16_t packetId);

    void publishDiscoveryMessage();
};

#endif