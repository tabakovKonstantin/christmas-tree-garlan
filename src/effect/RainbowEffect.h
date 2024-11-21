#ifndef RAINBOW_EFFECT_H
#define RAINBOW_EFFECT_H

#include "Effect.h"
#include <Ticker.h>

class RainbowEffect : public Effect {
private:
    uint8_t hue = 0;
    Ticker effectTimer;

    void internalRun(CRGB* leds, int numLeds);

public:
    void run(CRGB* leds, int numLeds) override;
};

#endif
