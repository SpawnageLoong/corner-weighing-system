# Corner Weighing System for Raspberry Pi Pico 2

A C++ application that reads weights from 4 HX711 load cells, displays mass per corner, total weight, and distribution percentages on a 128x64 I2C SSD1306 OLED. Includes support for tare and per-corner calibration using two buttons.

## ❗ AI Disclosure

The initial code for this project was generated using ChatGPT's 4o model on 25-May-2025. More details regarding the use of AI in this project can be found in the `Use of AI` section of the readme.

## 🧰 Hardware Requirements

- Raspberry Pi Pico or Pico 2
- 4 × HX711 + 1kg Load Cells
- SSD1306 128x64 OLED Display (I2C)
- 2 Push Buttons (for Tare and Calibration)
- Known calibration weight (100g)

## 📦 Dependencies

- [pico-sdk](https://github.com/raspberrypi/pico-sdk)
- CMake ≥ 3.13
- GCC ARM Toolchain

## 🔧 Building the Project

```bash
git clone https://github.com/yourusername/corner-weighing-system.git
cd corner-weighing-system

# Clone Pico SDK if not already available
git clone -b master https://github.com/raspberrypi/pico-sdk
export PICO_SDK_PATH=$PWD/pico-sdk

# Create and build
mkdir build && cd build
cmake ..
make
```

## 🤖 Use of AI

The initial code for this project was generated using ChatGPT's 4o model on 25-May-2025. The files generated are as follows:

- `CMakeLists.txt`
- `main.cpp`
- `hx711.h`
- `hx711.cpp`
- `ssd1306.h`
- `ssd1306.cpp`
- `font5x7.h`
- `README.md`

The code was then manually reviewed, tested, and modified to ensure correctness and functionality. The AI-generated code served as a starting point, which was refined to meet the specific requirements of this project.

GitHub Copilot is used for code completion and suggestions during development. 