#ifndef RECEIVER_H
#define RECEIVER_H

#include <Stdint.h>
#include "Arduino.h"

class Receiver {
    private:
        uint8_t clk;
        uint8_t receive;
    public:
        Receiver(uint8_t, uint8_t);
        uint8_t ReadByte();
};

#endif