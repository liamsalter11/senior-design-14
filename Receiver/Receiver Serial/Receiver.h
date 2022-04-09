#ifndef RECEIVER_H
#define RECEIVER_H

#include <Stdint.h>
#include "Arduino.h"

class Receiver {
    private:
        uint8_t clk;
        uint8_t receive;
        uint8_t r, g, b;
    public:
        Receiver(uint8_t, uint8_t);
        uint8_t ReadByte();
        uint8_t ReadByte2();
        void setRGB(uint8_t, uint8_t, uint8_t);
};

#endif