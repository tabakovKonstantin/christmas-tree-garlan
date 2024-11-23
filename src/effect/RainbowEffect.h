#ifndef RAINBOW_EFFECT_H
#define RAINBOW_EFFECT_H

#include "Effect.h"

class RainbowEffect : public Effect {
private:
    uint8_t hue = 0;

public:
    void run(CRGB* leds, int numLeds) override;
};

#endif
