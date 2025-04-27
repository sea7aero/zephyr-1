/*
  RadioLib LoRaWAN Starter Example

  ! Please refer to the included notes to get started !

  This example joins a LoRaWAN network and will send
  uplink packets. Before you start, you will have to
  register your device at https://www.thethingsnetwork.org/
  After your device is registered, you can run this example.
  The device will join the network and start uploading data.

  Running this examples REQUIRES you to check "Resets DevNonces"
  on your LoRaWAN dashboard. Refer to the network's
  documentation on how to do this.

  For default module settings, see the wiki page
  https://github.com/jgromes/RadioLib/wiki/Default-configuration

  For full API reference, see the GitHub Pages
  https://jgromes.github.io/RadioLib/

  For LoRaWAN details, see the wiki page
  https://github.com/jgromes/RadioLib/wiki/LoRaWAN

*/

#include <Arduino.h>
#include <LightTracker10.h>
#include <ThingNetwork.h>

// how often to send an uplink - consider legal & FUP constraints - see notes
const uint32_t uplinkIntervalSeconds = 5UL * 60UL;    // minutes x seconds

// create the LoRaWAN node
Module radioModule(PIN_NSS, PIN_DIO1, PIN_NRESET, PIN_BUSY);
SX1262 radio(&radioModule);
LoRaWANNode node(&radio, &US915, 2);

// result code to text - these are error codes that can be raised when using LoRaWAN
// however, RadioLib has many more - see https://jgromes.github.io/RadioLib/group__status__codes.html for a complete list
String stateDecode(const int16_t result)
{
    switch (result)
    {
    case RADIOLIB_ERR_NONE:
        return "ERR_NONE";
    case RADIOLIB_ERR_CHIP_NOT_FOUND:
        return "ERR_CHIP_NOT_FOUND";
    case RADIOLIB_ERR_PACKET_TOO_LONG:
        return "ERR_PACKET_TOO_LONG";
    case RADIOLIB_ERR_RX_TIMEOUT:
        return "ERR_RX_TIMEOUT";
    case RADIOLIB_ERR_CRC_MISMATCH:
        return "ERR_CRC_MISMATCH";
    case RADIOLIB_ERR_INVALID_BANDWIDTH:
        return "ERR_INVALID_BANDWIDTH";
    case RADIOLIB_ERR_INVALID_SPREADING_FACTOR:
        return "ERR_INVALID_SPREADING_FACTOR";
    case RADIOLIB_ERR_INVALID_CODING_RATE:
        return "ERR_INVALID_CODING_RATE";
    case RADIOLIB_ERR_INVALID_FREQUENCY:
        return "ERR_INVALID_FREQUENCY";
    case RADIOLIB_ERR_INVALID_OUTPUT_POWER:
        return "ERR_INVALID_OUTPUT_POWER";
    case RADIOLIB_ERR_NETWORK_NOT_JOINED:
        return "RADIOLIB_ERR_NETWORK_NOT_JOINED";
    case RADIOLIB_ERR_DOWNLINK_MALFORMED:
        return "RADIOLIB_ERR_DOWNLINK_MALFORMED";
    case RADIOLIB_ERR_INVALID_REVISION:
        return "RADIOLIB_ERR_INVALID_REVISION";
    case RADIOLIB_ERR_INVALID_PORT:
        return "RADIOLIB_ERR_INVALID_PORT";
    case RADIOLIB_ERR_NO_RX_WINDOW:
        return "RADIOLIB_ERR_NO_RX_WINDOW";
    case RADIOLIB_ERR_INVALID_CID:
        return "RADIOLIB_ERR_INVALID_CID";
    case RADIOLIB_ERR_UPLINK_UNAVAILABLE:
        return "RADIOLIB_ERR_UPLINK_UNAVAILABLE";
    case RADIOLIB_ERR_COMMAND_QUEUE_FULL:
        return "RADIOLIB_ERR_COMMAND_QUEUE_FULL";
    case RADIOLIB_ERR_COMMAND_QUEUE_ITEM_NOT_FOUND:
        return "RADIOLIB_ERR_COMMAND_QUEUE_ITEM_NOT_FOUND";
    case RADIOLIB_ERR_JOIN_NONCE_INVALID:
        return "RADIOLIB_ERR_JOIN_NONCE_INVALID";
    case RADIOLIB_ERR_N_FCNT_DOWN_INVALID:
        return "RADIOLIB_ERR_N_FCNT_DOWN_INVALID";
    case RADIOLIB_ERR_A_FCNT_DOWN_INVALID:
        return "RADIOLIB_ERR_A_FCNT_DOWN_INVALID";
    case RADIOLIB_ERR_DWELL_TIME_EXCEEDED:
        return "RADIOLIB_ERR_DWELL_TIME_EXCEEDED";
    case RADIOLIB_ERR_CHECKSUM_MISMATCH:
        return "RADIOLIB_ERR_CHECKSUM_MISMATCH";
    case RADIOLIB_ERR_NO_JOIN_ACCEPT:
        return "RADIOLIB_ERR_NO_JOIN_ACCEPT";
    case RADIOLIB_LORAWAN_SESSION_RESTORED:
        return "RADIOLIB_LORAWAN_SESSION_RESTORED";
    case RADIOLIB_LORAWAN_NEW_SESSION:
        return "RADIOLIB_LORAWAN_NEW_SESSION";
    case RADIOLIB_ERR_NONCES_DISCARDED:
        return "RADIOLIB_ERR_NONCES_DISCARDED";
    case RADIOLIB_ERR_SESSION_DISCARDED:
        return "RADIOLIB_ERR_SESSION_DISCARDED";
    }
    return "See https://jgromes.github.io/RadioLib/group__status__codes.html";
}

// helper function to display any issues
void debug(bool failed, const __FlashStringHelper *message, int state, bool halt)
{
    if (failed)
    {
        SerialUSB.print(message);
        SerialUSB.print(" - ");
        SerialUSB.print(stateDecode(state));
        SerialUSB.print(" (");
        SerialUSB.print(state);
        SerialUSB.println(")");
        while (halt)
        {
            delay(1);
        }
    }
}

// helper function to display a byte array
void arrayDump(uint8_t *buffer, uint16_t len)
{
    for (uint16_t c = 0; c < len; c++)
    {
        char b = buffer[c];
        if (b < 0x10)
        {
            SerialUSB.print('0');
        }
        SerialUSB.print(b, HEX);
    }
    SerialUSB.println();
}

void setup()
{
    SerialUSB.begin(115200);
    while (!Serial)
        ;
    delay(5000); // Give time to switch to the serial monitor
    SerialUSB.println(F("\nSetup ... "));

    SerialUSB.println(F("Initialise the radio"));
    int16_t state = radio.begin();
    debug(state != RADIOLIB_ERR_NONE, F("Initialise radio failed"), state, true);

    // Setup the OTAA session information
    state = node.beginOTAA(THING_NETWORK_JOIN_EUI, THING_NETWORK_DEV_EUI, THING_NETWORK_NWK_KEY, THING_NETWORK_APP_KEY);
    debug(state != RADIOLIB_ERR_NONE, F("Initialise node failed"), state, true);

    while (true)
    {
        SerialUSB.println(F("Join ('login') the LoRaWAN Network"));
        state = node.activateOTAA();
        debug(state != RADIOLIB_LORAWAN_NEW_SESSION, F("Join failed"), state, false);
        if (state == RADIOLIB_LORAWAN_NEW_SESSION)
        {
            break;
        }
        else
        {
            delay(5000);
        }
    }

    SerialUSB.println(F("Ready!\n"));
}

void loop()
{
    SerialUSB.println(F("Sending uplink"));

    // This is the place to gather the sensor inputs
    // Instead of reading any real sensor, we just generate some random numbers as example
    // Formatted as CayenneLLP
    uint8_t channel = 1;
    uint8_t type = 0x0; // type "Digital Input"
    uint8_t value = radio.randomByte();

    // Build payload byte array
    uint8_t uplinkPayload[3];
    uplinkPayload[0] = channel;
    uplinkPayload[1] = type;
    uplinkPayload[2] = value;

    // Perform an uplink
    int16_t state = node.sendReceive(uplinkPayload, sizeof(uplinkPayload));
    debug(state < RADIOLIB_ERR_NONE, F("Error in sendReceive"), state, false);

    // Check if a downlink was received
    // (state 0 = no downlink, state 1/2 = downlink in window Rx1/Rx2)
    if (state > 0)
    {
        SerialUSB.println(F("Received a downlink"));
    }
    else
    {
        SerialUSB.println(F("No downlink received"));
    }

    SerialUSB.print(F("Next uplink in "));
    SerialUSB.print(uplinkIntervalSeconds);
    SerialUSB.println(F(" seconds\n"));

    // Wait until next uplink - observing legal & TTN FUP constraints
    delay(uplinkIntervalSeconds * 1000UL); // delay needs milli-seconds
}