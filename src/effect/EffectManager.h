#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "Effect.h"

class EffectManager {
private:
    Effect* currentEffect = nullptr;

public:
    void setEffect(Effect* effect);
    void runEffect(CRGB* leds, int numLeds);

    ~EffectManager();
};

#endif