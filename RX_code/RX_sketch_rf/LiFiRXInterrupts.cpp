#include "LiFiRXInterrupts.hpp"
#include "LiFiRXConsts.hpp"

#include <Arduino.h>

volatile bool hitFrame = false;

void frameIntHandler()
{
	hitFrame = true;
}

void LiFiRXInterrupts::waitForFrameInterrupt()
{
  attachInterrupt(digitalPinToInterrupt(interruptPin), frameIntHandler, RISING);
  hitFrame = false;
  int timeout = millis() + frameTimeout;
  while (!hitFrame || micros() > timeout);
  detachInterrupt(digitalPinToInterrupt(interruptPin));
}
