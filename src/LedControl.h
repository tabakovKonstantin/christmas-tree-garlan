#ifndef LED_CONTROL_H
#define LED_CONTROL_H

 #include <stdint.h>
// #include <Arduino.h>
 #include <Payload.h>

void initLEDs();
void changeState(const Payload& payload);
void setLEDColor(uint32_t color);
void setLEDBrightness(int brightness);
void setLEDEffect(String effect);
void rainbowEffect();
// void getVoltage();

#endif
