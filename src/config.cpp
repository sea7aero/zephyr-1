#include "config.h"
#include "modules/SX126x/SX1262.h"

// Create a Module instance for the radio
Module radioModule(PIN_NSS, PIN_DIO1, PIN_NRESET, PIN_BUSY);

// Define the global objects
SX1262 radio(&radioModule);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
LoRaWANNode node(&radio, &Region, subBand);