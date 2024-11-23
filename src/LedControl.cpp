#include "LedControl.h"
#include <FastLED.h>

#define FASTLED_ESP8266_RAW_PIN_ORDER
#define LED_PIN 0
#define NUM_LEDS 200
#define BRIGHTNESS 2
#define LED_TYPE SK6812
#define COLOR_ORDER RGB
#define UPDATES_PER_SECOND 10

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

    if(payload.brightness != -1) {
        setLEDBrightness(payload.brightness);
      //  getVoltage();
    }

    // if (payload.color_mode == "rgb")
    // {
        if(payload.color.r  != -1 && payload.color.g  != -1 && payload.color.b != -1) {
            uint32_t color = (payload.color.r << 16) | (payload.color.g << 8) | payload.color.b;
            setLEDColor(color);
        }
        
    // }

    if (payload.effect != "")
    {
        setLEDEffect(payload.effect);
    }

    if (payload.state == "ON" && payload.color.r != -1)
    {
        Serial.println("Turn on");
        fill_solid(leds, NUM_LEDS, CRGB::White);
        // //  setLEDColor(CRGB::Black);
        // // FastLED.clear();
        setLEDBrightness(10);
        FastLED.show();
        
        // // FastLED.show();
    }

    if (payload.state == "OFF")
    {
        Serial.println("Turn off");
        // fill_solid(leds, NUM_LEDS, CRGB::DarkBlue);
        //  setLEDColor(CRGB::Black);
        FastLED.clear();
        FastLED.show();
        setLEDBrightness(0);
    }
}

void getVoltage() {
    float voltage = ESP.getVcc() / 1024.0; // Возвращает значение в мВ, преобразуем в В
    Serial.print("Напряжение питания: ");
    Serial.print(voltage);
    Serial.println(" В");
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
    rainbowEffect();
    // Реализация эффектов (например, мигание, радуга и т.д.)
}

void rainbowEffect() {
    uint8_t gHue = 0;
    // Проходимся по каждому светодиоду
    for (int i = 0; i < NUM_LEDS; i++) {
        // Устанавливаем цвет для каждого светодиода с учетом смещения gHue
        leds[i] = CHSV((i * 10 + gHue) % 255, 255, 255);
       
    }
    gHue++; // Изменяем смещение для создания эффекта движения
     FastLED.show();
}