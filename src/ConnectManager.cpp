#include "ConnectManager.h"
#include "ConfigManager.h"
#include <ESP8266WiFi.h>

Config config;
AsyncWebServer server(80);

const char *PARAM_SSID = "ssid";
const char *PARAM_PASSWORD = "password";
const char *PARAM_MQTT_URL = "mqtt_url";
const char *PARAM_MQTT_PORT = "mqtt_port";

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

void initWiFi()
{
    WiFi.mode(WIFI_STA);
    Serial.println();
    Serial.println("Initializing WiFi...");

    // ConfigManager::eraseConfig();
    bool tr = ConfigManager::loadConfig(config);
    if (tr)
    {
        Serial.println(tr);
        Serial.println("pass");
        Serial.println(config.wifiSSID);
        Serial.println(config.wifiPassword);

        WiFi.begin(config.wifiSSID, config.wifiPassword);

        unsigned long startAttemptTime = millis();
        const unsigned long timeout = 10000;

        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout)
        {
            delay(500);
            Serial.print(".");
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("\nConnected to Wi-Fi!");
            Serial.print("IP Address: ");
            Serial.println(WiFi.localIP());
            return;
        }

        Serial.println("\nFailed to connect to Wi-Fi.");
    }

    Serial.print(WiFi.status());
    WiFi.mode(WIFI_AP);
    WiFi.softAP("esp-captive");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              {  
        String html = "<h1>Wi-Fi Configuration</h1>";
html += "<form action='/config' method='POST'>";
html += "SSID: <input type='text' name='ssid'><br>";
html += "Password: <input type='password' name='password'><br>";
html += "MQTT Server URL: <input type='text' name='mqtt_url' value='192.168.100.115'><br>";
html += "MQTT Port: <input type='number' name='mqtt_port' value='1883'><br>";
html += "<input type='submit' value='Save'>";
html += "</form>";
        request->send(200, "text/html", html); });

    server.on("/config", HTTP_POST, handleConfigRequest);

    server.onNotFound(notFound);

    server.begin();
}

void handleConfigRequest(AsyncWebServerRequest *request)
{
    String ssid = "";
    String password = "";
    String mqttServer = "";
    int mqttPort;

    if (request->hasParam(PARAM_SSID, true))
    {
        ssid = request->getParam(PARAM_SSID, true)->value();
    }

    if (request->hasParam(PARAM_PASSWORD, true))
    {
        password = request->getParam(PARAM_PASSWORD, true)->value();
    }

    if (request->hasParam(PARAM_MQTT_URL, true))
    {
        mqttServer = request->getParam(PARAM_MQTT_URL, true)->value();
    }

    if (request->hasParam(PARAM_MQTT_PORT, true))
    {
        mqttPort = request->getParam(PARAM_MQTT_PORT, true)->value().toInt();
    }

    if (ssid.length() > 0 && password.length() > 0)
    {
        config.mqttServer = mqttServer;
        config.mqttPort = mqttPort;
        config.wifiSSID = ssid;
        config.wifiPassword = password;

        ConfigManager::saveConfig(config);

        request->send(200, "text/html", "<h1>Configuration Saved! Rebooting...</h1>");
        delay(2000);
        ESP.restart();
    }
    else
    {
        request->send(400, "text/html", "<h1>Missing SSID or Password</h1>");
    }
}

String getSsidWithChipId()
{
    uint32_t chipId = ESP.getChipId();
    char chipIdStr[11];
    itoa(chipId, chipIdStr, 10);

    String ssid = "Garland-" + String(chipIdStr);

    return ssid;
}