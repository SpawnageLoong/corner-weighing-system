#include <stdio.h>
#include "pico/stdlib.h"
#include "hx711.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include <cstring>  // for strlen


#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5

#define TARE_BUTTON 14
#define CAL_BUTTON 15

#define NUM_CORNERS 4
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

HX711 loadCells[NUM_CORNERS] = {
    HX711(2, 3),  // DOUT, SCK
    HX711(6, 7),
    HX711(8, 9),
    HX711(10, 11)
};

float calibrationFactor[NUM_CORNERS] = {1.0, 1.0, 1.0, 1.0};
long tareOffset[NUM_CORNERS] = {0};

int currentCalCorner = 0;
bool isCalibrating = false;

void displayWeights(SSD1306& display, float* weights, int calibratingCorner) {
    display.clear();

    float total = 0.0f;
    for (int i = 0; i < NUM_CORNERS; i++) total += weights[i];

    float pct[NUM_CORNERS];
    for (int i = 0; i < NUM_CORNERS; i++)
        pct[i] = total > 0 ? (weights[i] / total) * 100.0f : 0;

    // Compute distributions
    float left_pct   = pct[0] + pct[1]; // C1 + C2
    float right_pct  = pct[2] + pct[3]; // C3 + C4
    float front_pct  = pct[1] + pct[2]; // C2 + C3
    float rear_pct   = pct[0] + pct[3]; // C1 + C4

    char buf[32];

    // Draw platform box (48x32 in center)
    int box_x = 40, box_y = 16, box_w = 48, box_h = 32;
    display.draw_rect(box_x, box_y, box_w, box_h);

    // Draw corner weights at corners of the box
    snprintf(buf, sizeof(buf), "%.0fg", weights[1]); // C2 top-left
    display.draw_text(box_x - 38, box_y - 8, buf);

    snprintf(buf, sizeof(buf), "%.0fg", weights[2]); // C3 top-right
    display.draw_text(box_x + box_w + 2, box_y - 8, buf);

    snprintf(buf, sizeof(buf), "%.0fg", weights[0]); // C1 bottom-left
    display.draw_text(box_x - 38, box_y + box_h + 2, buf);

    snprintf(buf, sizeof(buf), "%.0fg", weights[3]); // C4 bottom-right
    display.draw_text(box_x + box_w + 2, box_y + box_h + 2, buf);

    // Center: total weight
    snprintf(buf, sizeof(buf), "T:%.0fg", total);
    display.draw_text((128 - strlen(buf) * 6) / 2, 30, buf);

    // Left and Right distributions (vertically beside box)
    snprintf(buf, sizeof(buf), "%.0f%%", left_pct);
    display.draw_text(box_x - 36, box_y + (box_h / 2) - 4, buf);

    snprintf(buf, sizeof(buf), "%.0f%%", right_pct);
    display.draw_text(box_x + box_w + 4, box_y + (box_h / 2) - 4, buf);

    // Top and Bottom distributions (horizontally)
    snprintf(buf, sizeof(buf), "%.0f%%", front_pct);
    display.draw_text(box_x + (box_w / 2) - 12, box_y - 10, buf);

    snprintf(buf, sizeof(buf), "%.0f%%", rear_pct);
    display.draw_text(box_x + (box_w / 2) - 12, box_y + box_h + 2, buf);

    // Draw calibration markers
    if (calibratingCorner == 0) display.draw_text(box_x - 40, box_y + box_h + 10, "↑");         // C1
    if (calibratingCorner == 1) display.draw_text(box_x - 40, box_y - 16, "↓");                 // C2
    if (calibratingCorner == 2) display.draw_text(box_x + box_w + 10, box_y - 16, "↓");         // C3
    if (calibratingCorner == 3) display.draw_text(box_x + box_w + 10, box_y + box_h + 10, "↑"); // C4

    display.display();
}



void tare() {
    for (int i = 0; i < NUM_CORNERS; i++) {
        tareOffset[i] = loadCells[i].read_average();
    }
}

void calibrate(float knownWeight) {
    printf("Calibrating corner %d\n", currentCalCorner + 1);
    long reading = loadCells[currentCalCorner].read_average();
    calibrationFactor[currentCalCorner] = (reading - tareOffset[currentCalCorner]) / knownWeight;
    currentCalCorner = (currentCalCorner + 1) % NUM_CORNERS;
}

int main() {
    stdio_init_all();
    for (int i = 0; i < NUM_CORNERS; i++) {
        loadCells[i].begin();
    }

    gpio_init(TARE_BUTTON);
    gpio_set_dir(TARE_BUTTON, GPIO_IN);
    gpio_pull_up(TARE_BUTTON);

    gpio_init(CAL_BUTTON);
    gpio_set_dir(CAL_BUTTON, GPIO_IN);
    gpio_pull_up(CAL_BUTTON);

    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    SSD1306 display(I2C_PORT, OLED_WIDTH, OLED_HEIGHT);
    display.init();
    display.clear();
    display.display();

    tare();

    while (true) {
        float weights[NUM_CORNERS];
        for (int i = 0; i < NUM_CORNERS; i++) {
            long raw = loadCells[i].read_average();
            weights[i] = (raw - tareOffset[i]) / calibrationFactor[i];
        }

        displayWeights(display, weights, -1);

        if (!gpio_get(TARE_BUTTON)) {
            tare();
            sleep_ms(500);
        }

        if (!gpio_get(CAL_BUTTON)) {
            calibrate(100.0); // Known weight = 100g (adjust as needed)
            sleep_ms(500);
        }

        sleep_ms(200);
    }
}
