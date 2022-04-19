#include "LiFiRXInterrupts.hpp"
#include "LiFiRXConsts.hpp"

#include <Arduino.h>

const int intPinBitMask = 1 << (13 - interruptPin);

void LiFiRXInterrupts::waitForFrameInterrupt()
{
  while (PORTB & intPinBitMask);
}
