#ifndef CONFIG_H
#define CONFIG_H

#include <RadioLib.h>
#include <Adafruit_SSD1306.h>

namespace zephyr
{

    // SX1262 LoRa Module has the following connections:
    // NSS pin:   8 - SPI Slave Select
    // DIO1 pin:  3 - IRQ
    // NRESET:    9 - RST
    // BUSY pin:  2 - GPIO
    constexpr uint8_t PIN_NSS = 8;
    constexpr uint8_t PIN_DIO1 = 3;
    constexpr uint8_t PIN_NRESET = 9;
    constexpr uint8_t PIN_BUSY = 2;

    // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
    constexpr uint8_t SCREEN_WIDTH = 128;    // OLED display width, in pixels
    constexpr uint8_t SCREEN_HEIGHT = 64;    // OLED display height, in pixels
    constexpr uint8_t SCREEN_ADDRESS = 0x3C; // Standard is 0x3D for 128x64, but my model is 0x3c

    // if you have RadioBoards (https://github.com/radiolib-org/RadioBoards)
    // and are using one of the supported boards, you can do the following:
    /*
    #define RADIO_BOARD_AUTO
    #include <RadioBoards.h>

    Radio radio = new RadioModule();
    */

    // how often to send an uplink - consider legal & FUP constraints - see notes
    constexpr uint32_t uplinkIntervalSeconds = 5UL * 60UL; // minutes x seconds

    // joinEUI - previous versions of LoRaWAN called this AppEUI
    // for development purposes you can use all zeros - see wiki for details
    constexpr uint64_t RADIOLIB_LORAWAN_JOIN_EUI = 0x0000000000000000;

// the Device EUI & two keys can be generated on the TTN console
#ifndef RADIOLIB_LORAWAN_DEV_EUI // Replace with your Device EUI
    constexpr uint64_t RADIOLIB_LORAWAN_DEV_EUI = 0x70B3D57ED006ED02;
#endif
#ifndef RADIOLIB_LORAWAN_APP_KEY // Replace with your App Key
    constexpr uint8_t RADIOLIB_LORAWAN_APP_KEY[] = {
        0x73, 0xA3, 0x1E, 0xB6, 0xBD, 0x9C, 0x20, 0x28,
        0xFE, 0x99, 0xCF, 0x0F, 0xBA, 0xA4, 0x9B, 0xCB};
#endif
#ifndef RADIOLIB_LORAWAN_NWK_KEY // Put your Nwk Key here
    constexpr uint8_t RADIOLIB_LORAWAN_NWK_KEY[] = {
        0xC5, 0x83, 0xDA, 0x50, 0x3A, 0xE3, 0xE0, 0x4B,
        0x52, 0x48, 0x17, 0xE1, 0xBE, 0x78, 0x8C, 0xAC};
#endif

    // for the curious, the #ifndef blocks allow for automated testing &/or you can
    // put your EUI & keys in to your platformio.ini - see wiki for more tips

    // regional choices: EU868, US915, AU915, AS923, AS923_2, AS923_3, AS923_4, IN865, KR920, CN470
    constexpr LoRaWANBand_t Region = US915;

    // subband choice: for US915/AU915 set to 2, for CN470 set to 1, otherwise leave on 0
    constexpr uint8_t subBand = 2;

    // ============================================================================
    // Below is to support the sketch - only make changes if the notes say so ...

    // copy over the EUI's & keys in to the something that will not compile if incorrectly formatted
    constexpr uint64_t joinEUI = RADIOLIB_LORAWAN_JOIN_EUI;
    constexpr uint64_t devEUI = RADIOLIB_LORAWAN_DEV_EUI;
    constexpr uint8_t appKey[] = {RADIOLIB_LORAWAN_APP_KEY};
    constexpr uint8_t nwkKey[] = {RADIOLIB_LORAWAN_NWK_KEY};

    // Global objects with static storage duration
    extern SX1262 radio;
    extern Adafruit_SSD1306 display;
    extern LoRaWANNode node;

} // namespace zephyr

#endif // CONFIG_H