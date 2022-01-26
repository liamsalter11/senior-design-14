#include "Transmit.h"

Transmit::Transmit(uint8_t clkPin, uint8_t transmitterPin) : clk(clkPin), transmitter(transmitterPin) {
    pinMode(clk, OUTPUT);
    pinMode(transmitter, OUTPUT);
    period = 1000000;
}

void Transmit::sendByte(uint8_t d) {
    uint8_t mask = 0b10000000;
    unsigned long t;
    for (uint8_t i = 0; i < 8; i++) {
        // Start clock period
        t = micros();
        digitalWrite(clk, HIGH);

        // Transmit bit
        uint8_t sendBit = mask & d;
        digitalWrite(transmitter, sendBit);

        // Adjust mask for next bit
        mask = mask >> 1;

        // Clock falls after half period
        while (micros() < (t + period / 2));
        digitalWrite(clk, LOW);

        // Clock stays low for half period
        while (micros() < (t + period));
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

void Transmit::sendByte2(uint8_t d) {
    uint8_t mask[] = {0b10000000, 0b01000000};
    unsigned long t;
    uint8_t sendR, sendB;
    for (uint8_t i = 0; i < 4; i++) {
        // Start clock period
        t = micros();
        digitalWrite(clk, HIGH);

        // Transmit bit
        sendR = mask[0] & d;
        sendB = mask[1] & d;
        digitalWrite(r, sendR);
        digitalWrite(b, sendB);

        // Adjust masks for next bit
        mask[0] = mask[0] >> 2;
        mask[1] = mask[1] >> 2;

        // Clock falls after half period
        while (micros() < (t + period / 2));
        digitalWrite(clk, LOW);

        // Clock stays low for half period
        while (micros() < (t + period));
    }
}

void Transmit::sendString2(String s) {
    for (char c : s) {
        sendByte2(c);
    }
}

void Transmit::setRGB(uint8_t red, uint8_t green, uint8_t blue) {
    r = red;
    g = green;
    b = blue;
    pinMode(r, OUTPUT);
    pinMode(g, OUTPUT);
    pinMode(b, OUTPUT);
}