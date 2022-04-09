#include "Receiver.h"

Receiver::Receiver(uint8_t clkPin, uint8_t receivePin) : clk(clkPin), receive(receivePin) {
    pinMode(clk, INPUT);
    pinMode(receive, INPUT);
}

uint8_t Receiver::ReadByte() {
    uint8_t readByte = 0;
  
    for (uint8_t i = 0; i < 8; i++) {
        // Hold so there is only one read per clock
        while (digitalRead(clk) ==LOW);
          uint8_t inBit = digitalRead(receive);
          //Serial.println("low");

        // Read in bit at falling clock edge
        
        
        readByte = (readByte << 1) ^ inBit;

        while (digitalRead(clk) == HIGH);
        // Shift bits then add incoming bit
    }
  
    return readByte;
}

uint8_t Receiver::ReadByte2() {
    uint8_t readByte = 0;
  
    for (uint8_t i = 0; i < 4; i++) {
        // Hold so there is only one read per clock
        while (analogRead(clk) <= 5);

        // Read in bits at falling clock edge
        while (analogRead(clk) > 5);
        uint8_t inBit = ((analogRead(r) > 4) << 1) | (analogRead(b) > 4);

        // Shift bits then add incoming bit
        readByte = (readByte << 2) ^ inBit;
    }
  
    return readByte;
}

void Receiver::setRGB(uint8_t red, uint8_t green, uint8_t blue) {
    r = red;
    g = green;
    b = blue;
    pinMode(r, INPUT);
    pinMode(g, INPUT);
    pinMode(b, INPUT);
}
