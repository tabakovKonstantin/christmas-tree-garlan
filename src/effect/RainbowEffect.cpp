#include "RainbowEffect.h"  

void RainbowEffect::run(CRGB* leds, int numLeds) {
    fill_rainbow(leds, numLeds, hue++);
}
