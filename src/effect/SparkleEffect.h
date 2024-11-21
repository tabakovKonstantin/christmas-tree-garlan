#include <FastLED.h>
#include "Effect.h"
#include <Ticker.h>

class SparkleEffect : public Effect
{
private:
    Ticker effectTimer;
    unsigned long lastUpdateTime = 0;
    int sparkleDelay = 80;   // Задержка для обновления эффекта
    int sparkleChance = 50; // Вероятность включения случайного светодиода
    int numSparkles = 10;

    void internalRun(CRGB *leds, int numLeds)
    {
        unsigned long currentMillis = millis();
        if (currentMillis - lastUpdateTime > sparkleDelay)
        {
            lastUpdateTime = currentMillis;

            fill_solid(leds, numLeds, CRGB::Black);
           
            if (random(0, 100) < sparkleChance)
            {
                for (int i = 0; i < numSparkles; i++)
                {
                    int randomIndex = random(0, numLeds);
                    leds[randomIndex] = CRGB::White;
                }
            }

            FastLED.show();
        }
    }

public:
    void run(CRGB *leds, int numLeds) override
    {
        effectTimer.attach_ms(100, [this, leds, numLeds]()
                              { internalRun(leds, numLeds); });
    }
};
