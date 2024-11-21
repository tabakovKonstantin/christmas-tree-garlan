#ifndef EFFECT_H
#define EFFECT_H

#include <FastLED.h>

class Effect {
public:
    virtual void run(CRGB* leds, int numLeds) = 0;
    virtual ~Effect() {}
};

#endif
