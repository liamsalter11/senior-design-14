#ifndef TRANSMIT_H
#define TRANSMIT_H

#include <Stdint.h>
#include "Arduino.h"

class Transmit {
    private:
        uint8_t clk;
        uint8_t transmitter;
        int period; // in milliseconds
    public:
        Transmit(uint8_t, uint8_t);
        void sendByte(uint8_t);
        void setPeriod(int);
        void sendString(String);
};

#endif
