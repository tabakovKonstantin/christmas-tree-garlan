#ifndef MqttManager_h
#define MqttManager_h

#include "Config.h"
#include <AsyncMqttClient.h>

class MqttManager
{
public:
    MqttManager();
    void init();

private:
    Config config;

    void connectToMqtt();
    void onMqttConnect(bool sessionPresent);
    void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
    void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
    void publishDiscoveryMessage();
};

#endif