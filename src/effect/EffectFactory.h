#ifndef EFFECT_FACTORY_H
#define EFFECT_FACTORY_H

#include "Effect.h"
#include "RainbowEffect.h"
#include "SparkleEffect.h"

class EffectFactory
{
public:
    static Effect *createEffect(const String &effectName)
    {
        // if (effectName == "rainbow")
        if (effectName == "effect2")
        {
            return new RainbowEffect();
        }
        if (effectName == "effect1") {
            return new SparkleEffect();
        }

        return nullptr;
    }
};

#endif
