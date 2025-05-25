#pragma once

#include "hardware/i2c.h"

class SSD1306 {
public:
    SSD1306(i2c_inst_t *i2c, uint8_t width, uint8_t height, uint8_t addr = 0x3C);
    void init();
    void clear();
    void display();
    void draw_text(int x, int y, const char *text);
    void set_pixel(int x, int y, bool on);
    void draw_line(int x0, int y0, int x1, int y1);
    void draw_rect(int x, int y, int w, int h);


private:
    i2c_inst_t* i2c;
    uint8_t width;
    uint8_t height;
    uint8_t address;
    uint8_t buffer[1024];

    void send_command(uint8_t cmd);
    void send_data(uint8_t* data, size_t len);
};
