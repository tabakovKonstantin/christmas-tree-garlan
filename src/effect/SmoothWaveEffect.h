#include <FastLED.h>
#include "Effect.h"
#include <Ticker.h>

class SmoothWaveEffect : public Effect
{
private:
    Ticker effectTimer;
    uint8_t hue = 0;        // Начальный оттенок
    unsigned long lastUpdateTime = 0;
    int updateDelay = 50; // Интервал обновления в миллисекундах
    uint8_t minBrightness = 10; // Минимальная яркость


    void internalRun(CRGB *leds, int numLeds)
    {
        unsigned long currentMillis = millis();
        if (currentMillis - lastUpdateTime > updateDelay)
        {
            lastUpdateTime = currentMillis;


            for (int i = 0; i < numLeds; i++)
            {
                // Вычисляем яркость по синусоидальной функции
                uint8_t wave = sin8((i * 10) + hue);
                // Ограничиваем яркость в диапазоне [minBrightness, 255]
                uint8_t adjustedBrightness = map(wave, 0, 255, minBrightness, 255);
                adjustedBrightness = constrain(adjustedBrightness, 0, 255);
                leds[i] = CHSV(hue, 255, adjustedBrightness); // Задаём цвет с плавным изменением яркости
            }

            hue++; // Постепенно изменяем оттенок для плавного перехода
            //FastLED.show();
        }
    }

public:
    void run(CRGB *leds, int numLeds) override
    {
        effectTimer.attach_ms(20, [this, leds, numLeds]()
                              { internalRun(leds, numLeds); });
    }
};
