#ifndef MqttManager_h
#define MqttManager_h

#include "Config.h"
#include "LedControl.h"
#include <AsyncMqttClient.h>

#define DISCOVERY_TOPIC_TEMPLATE "homeassistant/light/%s/config"
#define COMMAND_TOPIC_TEMPLATE "home/lights/%s/set"


class MqttManager
{
public:
    MqttManager(LedControl& led_Control);
    void init();

private:
    Config config;
    LedControl& ledControl;

    void connectToMqtt();
    void onMqttConnect(bool sessionPresent);
    void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
    void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
    void publishDiscoveryMessage();

    String getProductId();
    String getDiscoveryTopic();
    String getCommandTopic();

};

#endif