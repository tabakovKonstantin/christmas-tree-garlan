#include "LedControl.h"
#include <FastLED.h>

#define FASTLED_ESP8266_RAW_PIN_ORDER
#define LED_PIN 0
#define NUM_LEDS 200
#define BRIGHTNESS 2
#define LED_TYPE SK6812
#define COLOR_ORDER RGB
#define UPDATES_PER_SECOND 100

CRGB leds[NUM_LEDS];

void initLEDs()
{

    delay(3000);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear(true);
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void changeState(const Payload &payload)
{

    setLEDBrightness(payload.brightness);

    if (payload.color_mode == "rgb")
    {
        uint32_t color = (payload.color.r << 16) | (payload.color.g << 8) | payload.color.b;
        setLEDColor(color);
    }

    if (payload.effect != "")
    {
        setLEDEffect(payload.effect);
    }

    if (payload.state == "ON")
    {
        Serial.println("Turn on");
        FastLED.show();
    }

    if (payload.state == "OFF")
    {
        Serial.println("Turn off");
        FastLED.clear();
        FastLED.show();
    }
}

void setLEDColor(uint32_t color)
{
    Serial.println("Set color");
    Serial.println(color);
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = color;
    }
    FastLED.show();
}

void setLEDBrightness(int brightness)
{
    Serial.println("Set brightness");
    Serial.println(brightness);
    FastLED.setBrightness(brightness);
    FastLED.show();
}

void setLEDEffect(String effect)
{
    Serial.println("Set effect");
    Serial.println(effect);
    // Реализация эффектов (например, мигание, радуга и т.д.)
}
