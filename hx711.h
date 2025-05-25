#pragma once

#include "pico/stdlib.h"

class HX711 {
private:
    uint dataPin;
    uint clkPin;
public:
    HX711(uint dataPin, uint clkPin);
    void begin();
    long read();
    long read_average(uint times = 10);
};
