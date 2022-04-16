#include "Receiver.h"

Receiver::Receiver(uint8_t clkPin, uint8_t receivePin) : clk(clkPin), receive(receivePin) 
{
    pinMode(clk, INPUT);
    pinMode(receive, INPUT);
}

uint8_t Receiver::ReadByte() 
{
    uint8_t readByte = 0;
  
    for (uint8_t i = 0; i < 8; i++) {
        while (digitalRead(clk) ==LOW);
          uint8_t inBit = digitalRead(receive);
        readByte = (readByte << 1) ^ inBit;

        while (digitalRead(clk) == HIGH);
    }
  
    return readByte;
}

void Receiver::setRGB(uint8_t red, uint8_t green, uint8_t blue) 
{
    r = red;
    g = green;
    b = blue;
    pinMode(r, INPUT);
    pinMode(g, INPUT);
    pinMode(b, INPUT);
}
