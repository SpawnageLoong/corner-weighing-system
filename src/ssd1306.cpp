#include "ssd1306.h"
#include "pico/stdlib.h"
#include <string.h>
#include <stdio.h>
#include <cstdlib> // for abs()
#include <cmath>
#include "font5x7.h" // Include the font header file


// 5x7 font (basic ASCII 32-127)
extern const uint8_t font5x7[][5];

SSD1306::SSD1306(i2c_inst_t *i2cInst, uint8_t w, uint8_t h, uint8_t addr)
    : i2c(i2cInst), width(w), height(h), address(addr) {
    memset(buffer, 0, sizeof(buffer));
}

void SSD1306::send_command(uint8_t cmd) {
    uint8_t buf[2] = {0x00, cmd};
    i2c_write_blocking(i2c, address, buf, 2, false);
}

void SSD1306::send_data(uint8_t* data, size_t len) {
    uint8_t *buf = new uint8_t[len + 1];
    buf[0] = 0x40;
    memcpy(buf + 1, data, len);
    i2c_write_blocking(i2c, address, buf, len + 1, false);
    delete[] buf;
}

void SSD1306::init() {
    send_command(0xAE); // display off
    send_command(0x20); send_command(0x00); // horizontal addressing mode
    send_command(0xB0); // page 0
    send_command(0xC8); // COM scan direction
    send_command(0x00); // low column address
    send_command(0x10); // high column address
    send_command(0x40); // start line address
    send_command(0x81); send_command(0x7F); // contrast
    send_command(0xA1); // segment remap
    send_command(0xA6); // normal display
    send_command(0xA8); send_command(0x3F); // multiplex
    send_command(0xA4); // resume RAM content
    send_command(0xD3); send_command(0x00); // display offset
    send_command(0xD5); send_command(0x80); // clock
    send_command(0xD9); send_command(0xF1); // precharge
    send_command(0xDA); send_command(0x12); // com pins
    send_command(0xDB); send_command(0x40); // vcom detect
    send_command(0x8D); send_command(0x14); // charge pump
    send_command(0xAF); // display on
}

void SSD1306::clear() {
    memset(buffer, 0, sizeof(buffer));
}

void SSD1306::display() {
    for (uint8_t page = 0; page < 8; page++) {
        send_command(0xB0 + page);
        send_command(0x00);
        send_command(0x10);
        send_data(&buffer[page * 128], 128);
    }
}

void SSD1306::draw_text(int x, int y, const char *text) {
    int page = y / 8;
    while (*text && x < width - 6) {
        if (*text >= 32 && *text <= 127) {
            int ch = *text - 32;
            for (int i = 0; i < 5; i++) {
                buffer[page * 128 + x++] = font5x7[ch][i];
            }
            buffer[page * 128 + x++] = 0; // space
        }
        text++;
    }
}

void SSD1306::set_pixel(int x, int y, bool on) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    int page = y / 8;
    int bit = y % 8;
    if (on)
        buffer[page * width + x] |= (1 << bit);
    else
        buffer[page * width + x] &= ~(1 << bit);
}

// Bresenham's line algorithm
void SSD1306::draw_line(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (true) {
        set_pixel(x0, y0, true);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}


void SSD1306::draw_rect(int x, int y, int w, int h) {
    draw_line(x, y, x + w - 1, y);
    draw_line(x, y, x, y + h - 1);
    draw_line(x + w - 1, y, x + w - 1, y + h - 1);
    draw_line(x, y + h - 1, x + w - 1, y + h - 1);
}
