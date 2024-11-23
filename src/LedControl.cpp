#include "LedControl.h"
#include <FastLED.h>

#define FASTLED_ESP8266_RAW_PIN_ORDER
#define LED_PIN 0
#define NUM_LEDS 200
#define BRIGHTNESS 10
#define LED_TYPE SK6812
#define COLOR_ORDER RGB
#define UPDATES_PER_SECOND 10

CRGB leds[NUM_LEDS];

void initLEDs()
{
    Serial.println();
    Serial.println("Initializing LEDs...");

    delay(1000);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.delay(1000 / UPDATES_PER_SECOND);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear(true);
    FastLED.show();
}

void changeState(const Payload &payload)
{

    Serial.println();
    Serial.println("Applaying new state ..");

    if (payload.brightness != -1)
    {
        setLEDBrightness(payload.brightness);
    }

    if (payload.color.r != -1 && payload.color.g != -1 && payload.color.b != -1)
    {
        uint32_t color = (payload.color.r << 16) | (payload.color.g << 8) | payload.color.b;
        setLEDColor(color);
    }

    if (payload.effect != "null")
    {
        setLEDEffect(payload.effect);
    }

    if (payload.state == "ON" && payload.color.r == -1 && payload.brightness == -1 && payload.effect == "null")
    {
        Serial.println();
        Serial.println("Turn on");
        fill_solid(leds, NUM_LEDS, CRGB::White);
    }

    if (payload.state == "OFF")
    {
        Serial.println();
        Serial.println("Turn off");
        FastLED.clear();
    }
    Serial.println();
    Serial.println("Applaying has finished");
}

void setLEDColor(uint32_t color)
{
    Serial.println();
    Serial.print("Set color: ");
    Serial.println(color);
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = color;
    }
}

void setLEDBrightness(int brightness)
{
    Serial.println();
    Serial.print("Set brightness: ");
    Serial.println(brightness);
    FastLED.setBrightness(brightness);
}

void setLEDEffect(String effect)
{
    Serial.println();
    Serial.print("Set effect: ");
    Serial.println(effect);
    rainbowEffect();
}

void rainbowEffect()
{
    static uint8_t gHue = 0;
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CHSV((i * 10 + gHue) % 255, 255, 255);
    }
    gHue++;
}