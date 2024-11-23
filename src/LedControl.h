#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <FastLED.h>
#include "effect/EffectManager.h"
#include "Payload.h"

#define LED_PIN 0
#define NUM_LEDS 200
#define BRIGHTNESS 10
#define LED_TYPE SK6812
#define COLOR_ORDER RGB
#define UPDATES_PER_SECOND 10

class LedControl {
private:
    CRGB leds[NUM_LEDS];
    EffectManager& effectManager;

public:
    LedControl(EffectManager& manager);
    void initLEDs();
    void changeState(const Payload& payload);
    void setLEDColor(uint32_t color);
    void setLEDBrightness(int brightness);
    void setLEDEffect(String effect);
};

#endif
