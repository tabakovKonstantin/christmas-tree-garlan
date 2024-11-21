#include "EffectManager.h"

void EffectManager::setEffect(Effect* effect) {
    if (currentEffect) {
        delete currentEffect;
    }
    currentEffect = effect;
}

void EffectManager::runEffect(CRGB* leds, int numLeds) {
    if (currentEffect) {
        currentEffect->run(leds, numLeds);
    }
}

EffectManager::~EffectManager() {
    if (currentEffect) {
        delete currentEffect;
    }
}
