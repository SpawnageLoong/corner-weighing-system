#include "hx711.h"

HX711::HX711(uint data, uint clk) : dataPin(data), clkPin(clk) {}

void HX711::begin() {
    gpio_init(dataPin);
    gpio_set_dir(dataPin, GPIO_IN);
    gpio_init(clkPin);
    gpio_set_dir(clkPin, GPIO_OUT);
}

long HX711::read() {
    while (gpio_get(dataPin));
    long count = 0;
    for (int i = 0; i < 24; i++) {
        gpio_put(clkPin, 1);
        sleep_us(1);
        count = count << 1;
        gpio_put(clkPin, 0);
        if (gpio_get(dataPin)) count++;
        sleep_us(1);
    }
    gpio_put(clkPin, 1); sleep_us(1);
    gpio_put(clkPin, 0); sleep_us(1);
    count ^= 0x800000;
    return count;
}

long HX711::read_average(uint times) {
    long sum = 0;
    for (uint i = 0; i < times; i++) {
        sum += read();
    }
    return sum / times;
}
