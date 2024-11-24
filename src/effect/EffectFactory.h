#ifndef EFFECT_FACTORY_H
#define EFFECT_FACTORY_H

#include "Effect.h"
#include "RainbowEffect.h"
#include "SparkleEffect.h"
#include "SmoothWaveEffect.h"

class EffectFactory
{
public:
    static Effect *createEffect(const String &effectName)
    {
        if (effectName == "Rainbow")
        {
            return new RainbowEffect();
        }

        if (effectName == "Smooth wave") {
            return new SmoothWaveEffect();
        }

        if (effectName == "Sparkle")
        {
            return new SparkleEffect();
        }

        return nullptr;
    }
};

#endif
