#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <ArduinoJson.h>

class Payload
{
public:
    int brightness;
    String color_mode;
    int color_temp;
    struct Color
    {
        int r, g, b, c, w;
    } color;
    String effect;
    String state;
    int transition;

    String toJson() const
    {
        JsonDocument doc;
        doc["brightness"] = brightness;
        doc["color_mode"] = color_mode;
        doc["color_temp"] = color_temp;
        doc["color"]["r"] = color.r;
        doc["color"]["g"] = color.g;
        doc["color"]["b"] = color.b;
        doc["effect"] = effect;
        doc["state"] = state;
        doc["transition"] = transition;

        String output;
        serializeJson(doc, output);
        return output;
    }

    bool fromJson(const String &json)
    {
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, json);
        if (error)
        {
            Serial.println("Ошибка десериализации JSON");
            return false;
        }

        brightness = doc["brightness"].isNull() ? -1 : doc["brightness"];
        color_mode = doc["color_mode"].as<String>();
        color_temp = doc["color_temp"];
        color.r = doc["color"]["r"].isNull() ? -1 : doc["color"]["r"];
        color.g = doc["color"]["g"].isNull() ? -1 : doc["color"]["g"];
        color.b = doc["color"]["b"].isNull() ? -1 : doc["color"]["b"];
        effect = doc["effect"].as<String>();
        state = doc["state"].as<String>();
        transition = doc["transition"];

        return true;
    }
};

#endif // PAYLOAD_H
