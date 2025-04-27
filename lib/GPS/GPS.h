#pragma once

#include <SparkFun_Ublox_Arduino_Library.h>
#include <Arduino.h>


class GPS : public SFE_UBLOX_GPS {
private:
    pin_size_t enablePin_;

public:
    GPS(pin_size_t &enablePin) : enablePin_{enablePin} {}
    
    bool begin();
    void configure();
    void enable();
    void disable();



};