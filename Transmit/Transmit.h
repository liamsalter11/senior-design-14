#ifndef TRANSMIT_H
#define TRANSMIT_H

#include <Stdint.h>
#include "Arduino.h"

class Transmit {
    private:
        uint8_t clk;
        uint8_t transmitter;
        int period; // in microseconds
        uint8_t r, g, b;
    public:
        Transmit(uint8_t, uint8_t);
        void sendByte(uint8_t);
        void setPeriod(int);
        void sendString(String);
        void sendByte2(uint8_t);
        void sendString2(String);
        void setRGB(uint8_t, uint8_t, uint8_t);
};

#endif
