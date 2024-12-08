#include "ConnectManager.h"
#include "ConfigManager.h"
#include <ESP8266WiFi.h>

#include <ESPAsyncTCP.h>
#include "ESPAsyncWebServer.h"

Config config;
AsyncWebServer server(80);

const char *ssid = "YOUR_SSID";
const char *password = "YOUR_PASSWORD";

const char *PARAM_MESSAGE = "message";

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

void initWiFi()
{
    Serial.println();
    Serial.println("Initializing WiFi...");

    const char *ssid = "1A1_CA2F";
    const char *password = "48575443202C3FAA";

    WiFi.begin(ssid, password);
    // while (WiFi.status() != WL_CONNECTED)
    // {
    //     delay(1000);
    //     Serial.println("Connecting to WiFi...");
    // }
    if (WiFi.waitForConnectResult() == WL_CONNECTED)
    {
        Serial.printf("WiFi Succ!\n");
        return;
    }
    
    Serial.print(WiFi.status());
    WiFi.mode(WIFI_AP);
    WiFi.softAP("esp-captive");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "Hello, world"); });

    // Send a GET request to <IP>/get?message=<message>
    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        String message;
         Serial.print(" test1 ");
         config.mqttServer = "192.168.100.115";
        config.mqttPort = 1883;

        ConfigManager::saveConfig(config);

        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, GET: " + message); });

    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request)
              {
        String message;
        if (request->hasParam(PARAM_MESSAGE, true)) {
            message = request->getParam(PARAM_MESSAGE, true)->value();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "Hello, POST: " + message); });

    server.onNotFound(notFound);

    server.begin();
}

String getSsidWithChipId()
{
    uint32_t chipId = ESP.getChipId();
    char chipIdStr[11];
    itoa(chipId, chipIdStr, 10);

    String ssid = "Garland-" + String(chipIdStr);

    return ssid;
}