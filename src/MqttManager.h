#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <Config.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

class MqttManager
{
public:
    MqttManager();
    void initMQTT();
    void publish(const char *topic, const char *message);
    void handleMQTT();

private:
    Config config;
    WiFiClient espClient;
    PubSubClient client;

    void callback(char *topic, byte *payload, unsigned int length);
    void publishDiscoveryMessage();
};

#endif // MQTT_MANAGER_H
