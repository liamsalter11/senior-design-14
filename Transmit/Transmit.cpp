#include "Transmit.h"

Transmit::Transmit(uint8_t clkPin, uint8_t transmitterPin) : clk(clkPin), transmitter(transmitterPin) {
    pinMode(clk, OUTPUT);
    pinMode(transmitter, OUTPUT);
    period = 1000;
}

void Transmit::sendByte(uint8_t d) {
    uint8_t mask = 0b10000000;
    unsigned long t;
    for (uint8_t i = 0; i < 8; i++) {
        // Start clock period
        t = millis();
        digitalWrite(clk, HIGH);

        // Transmit bit
        uint8_t sendBit = mask & d;
        digitalWrite(transmitter, sendBit);

        // Adjust mask for next bit
        mask = mask >> 1;

        // Clock falls after half period
        while (millis() < (t + period / 2));
        digitalWrite(clk, LOW);

        // Clock stays low for half period
        while (millis() < (t + period));
    }
}

void Transmit::setPeriod(int p) {
    period = p;
}

void Transmit::sendString(String s) {
    for (char c : s) {
        sendByte(c);
    }
}