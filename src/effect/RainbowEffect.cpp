#include "RainbowEffect.h"

void RainbowEffect::run(CRGB *leds, int numLeds)
{
    effectTimer.attach_ms(500, [this, leds, numLeds]()
                          { internalRun(leds, numLeds); });
}

void RainbowEffect::internalRun(CRGB *leds, int numLeds)
{
    fill_rainbow(leds, numLeds, hue++, 15);
    Serial.println("Set effect: ");
    Serial.println(hue);
}
