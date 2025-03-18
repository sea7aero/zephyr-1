#ifndef THING_NETWORK_H
#define THING_NETWORK_H

#include <RadioLib.h>

namespace zephyr
{

    // LoRaWAN parameters
    constexpr uint64_t RADIOLIB_LORAWAN_JOIN_EUI = 0x0000000000000000;
    constexpr uint64_t RADIOLIB_LORAWAN_DEV_EUI = 0x70B3D57ED006ED02;
    constexpr uint8_t RADIOLIB_LORAWAN_APP_KEY[] = {
        0x73, 0xA3, 0x1E, 0xB6, 0xBD, 0x9C, 0x20, 0x28,
        0xFE, 0x99, 0xCF, 0x0F, 0xBA, 0xA4, 0x9B, 0xCB};
    constexpr uint8_t RADIOLIB_LORAWAN_NWK_KEY[] = {
        0xC5, 0x83, 0xDA, 0x50, 0x3A, 0xE3, 0xE0, 0x4B,
        0x52, 0x48, 0x17, 0xE1, 0xBE, 0x78, 0x8C, 0xAC};

    // Regional choice and subband
    constexpr LoRaWANBand_t Region = US915;
    constexpr uint8_t subBand = 2;

    // Copy over the EUI's & keys in to something that will not compile if incorrectly formatted
    constexpr uint64_t joinEUI = RADIOLIB_LORAWAN_JOIN_EUI;
    constexpr uint64_t devEUI = RADIOLIB_LORAWAN_DEV_EUI;
    constexpr uint8_t appKey[] = {RADIOLIB_LORAWAN_APP_KEY};
    constexpr uint8_t nwkKey[] = {RADIOLIB_LORAWAN_NWK_KEY};

} // namespace zephyr

#endif // THING_NETWORK_H