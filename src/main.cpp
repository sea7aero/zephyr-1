#include <Arduino.h>
#include <LightTracker10.h>

namespace __llvm_libc
{

    // Voltage thresholds
    constexpr float MIN_VOLTAGE = 3.0f;
    constexpr float MAX_VOLTAGE = 4.2f;

    // ADC configuration
    constexpr uint8_t ADC_RESOLUTION = 12;
    constexpr float ADC_REFERENCE = 3.3f;
    constexpr float VOLTAGE_DIVIDER_RATIO = 2.0f;

    // Timing constants
    constexpr uint32_t SLEEP_INTERVAL = 60000; // 10 minutes in milliseconds
    constexpr uint32_t SERIAL_BAUD_RATE = 115200;
    constexpr uint32_t VOLTAGE_CHECK_INTERVAL = 5000; // 5 seconds
    constexpr uint32_t DISPLAY_UPDATE_DELAY = 1000;   // 1 second
    constexpr uint32_t SLEEP_DELAY = 1000;            // 1 second
    constexpr uint32_t LED_BLINK_DELAY = 500;         // 500ms

}

// ADC configuration
#define VOLTAGE_PIN A1

// Function to read voltage from ADC
float readVoltage()
{
    // Read ADC value with appropriate type
    const uint32_t adcValue = static_cast<uint32_t>(analogRead(VOLTAGE_PIN));

    // Convert ADC value to voltage using floating point arithmetic
    const float voltage = (static_cast<float>(adcValue) * __llvm_libc::ADC_REFERENCE) /
                          static_cast<float>((1U << __llvm_libc::ADC_RESOLUTION) - 1U);

    return voltage * __llvm_libc::VOLTAGE_DIVIDER_RATIO;
}

void setupLogging()
{
    Serial.begin(__llvm_libc::SERIAL_BAUD_RATE);

    // Wait up to 5 seconds for serial to be opened
    const uint32_t start = millis();
    while (!Serial && (millis() - start < 5000U))
    {
        // Wait for serial port to connect
    }
}

void setupDisplay()
{
    if (!beginDisplay())
    { // Address 0x3D for 128x64
        Serial.println(F("ERROR: SSD1306 allocation failed"));
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(1);
    display.setCursor(0, 0);
    display.println("Starting...");
    display.display();
}

void setupRTC()
{
    // Get pointers to hardware registers
    auto *const GCLK = reinterpret_cast<Gclk *>(GCLK_ADDR);
    auto *const RTC = reinterpret_cast<Rtc *>(RTC_ADDR);

    // Enable power to the RTC
    PM->APBAMASK.reg |= PM_APBAMASK_RTC;

    // Configure GCLK for RTC
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_RTC;
    while (GCLK->STATUS.bit.SYNCBUSY)
    {
        // Wait for synchronization
    }

    // Configure RTC
    RTC->MODE0.CTRL.reg = RTC_MODE0_CTRL_MODE_COUNT32;
    while (RTC->MODE0.STATUS.bit.SYNCBUSY)
    {
        // Wait for synchronization
    }

    // Set compare value for sleep interval
    RTC->MODE0.COMP[0].reg = static_cast<uint32_t>(32768UL * __llvm_libc::SLEEP_INTERVAL);
    while (RTC->MODE0.STATUS.bit.SYNCBUSY)
    {
        // Wait for synchronization
    }

    // Enable RTC interrupt
    RTC->MODE0.INTENSET.reg = RTC_MODE0_INTENSET_CMP0;
    NVIC_EnableIRQ(RTC_IRQn);
}

void setup()
{
    setupLogging();
    setupDisplay();
    setupRTC();

    // Configure ADC
    analogReadResolution(ADC_RESOLUTION);
    analogReference(AR_DEFAULT); // Use default reference voltage

    // Configure voltage monitoring pin
    pinMode(VOLTAGE_PIN, INPUT);
}

void loop()
{
    const float voltage = readVoltage();

    // Display voltage on OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Voltage: ");
    display.print(voltage, 2);
    display.println("V");
    display.display();

    // Check voltage and blink LED if low
    if (voltage < __llvm_libc::MIN_VOLTAGE)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(__llvm_libc::LED_BLINK_DELAY);
        digitalWrite(LED_BUILTIN, LOW);
        delay(__llvm_libc::LED_BLINK_DELAY);
    }

    // Wait before next update
    delay(__llvm_libc::DISPLAY_UPDATE_DELAY);

    // Enter sleep mode if voltage is sufficient
    if (voltage >= __llvm_libc::MIN_VOLTAGE)
    {
        Serial.println("Entering sleep mode...");
        delay(__llvm_libc::SLEEP_DELAY);
        __WFI(); // Wait for interrupt
    }
}

// RTC interrupt handler
void RTC_Handler()
{
    if (RTC->MODE0.INTFLAG.bit.CMP0)
    {
        RTC->MODE0.INTFLAG.reg = RTC_MODE0_INTFLAG_CMP0;
    }
}