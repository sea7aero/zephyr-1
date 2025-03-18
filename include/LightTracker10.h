#ifndef LIGHTTRACKER10_H
#define LIGHTTRACKER10_H

/**
 * Configuration for the LightTracker v1.0 board
 * See: https://github.com/lightaprs/LightTracker-1.0
 */

#include <stdint.h>

// SX1262 LoRa Module has the following connections:
// NSS pin:   8 - SPI Slave Select
// DIO1 pin:  3 - IRQ
// NRESET:    9 - RST
// BUSY pin:  2 - GPIO
#include <RadioLib.h>

namespace zephyr
{

    constexpr uint8_t PIN_NSS = 8;
    constexpr uint8_t PIN_DIO1 = 3;
    constexpr uint8_t PIN_NRESET = 9;
    constexpr uint8_t PIN_BUSY = 2;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#include <Adafruit_SSD1306.h>

    constexpr uint8_t SCREEN_WIDTH = 128;    // OLED display width, in pixels
    constexpr uint8_t SCREEN_HEIGHT = 64;    // OLED display height, in pixels
    constexpr uint8_t SCREEN_ADDRESS = 0x3C; // Standard is 0x3D for 128x64, but my model is 0x3c

    // Global objects with static storage duration
    extern SX1262 radio;
    extern Adafruit_SSD1306 display;

    inline void beginDisplay()
    {
        display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.display();
    }

} // namespace zephyr

#endif // LIGHTTRACKER10_H