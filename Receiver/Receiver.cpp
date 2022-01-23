#include "Receiver.h"

Receiver::Receiver(uint8_t clkPin, uint8_t receivePin) : clk(clkPin), receive(receivePin) {
    pinMode(clk, INPUT);
    pinMode(receive, INPUT);
}

uint8_t Receiver::ReadByte() {
    uint8_t readByte = 0;
  
    for (uint8_t i = 0; i < 8; i++) {
        // Hold so there is only one read per clock
        while (analogRead(clk) <= 5);

        // Read in bit at falling clock edge
        while (analogRead(clk) > 5);
        uint8_t inBit = analogRead(receive) > 5;

        // Shift bits then add incoming bit
        readByte = (readByte << 1) ^ inBit;
    }
  
    return readByte;
}