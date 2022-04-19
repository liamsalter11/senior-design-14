#include "LiFiRXInterrupts.hpp"
#include "LiFiRXConsts.hpp"

#include <Arduino.h>

volatile bool hitFrame = false;

void frameIntHandler()
{
	hitFrame = true;
}

void LiFiRXInterrupts::enableFrameInterrupt()
{
	attachInterrupt(digitalPinToInterrupt(interruptPin), frameIntHandler, RISING);
  hitFrame = false;
}

void LiFiRXInterrupts::disableFrameInterrupt()
{
	detachInterrupt(digitalPinToInterrupt(interruptPin));
}

void LiFiRXInterrupts::waitForFrameInterrupt()
{
  attachInterrupt(digitalPinToInterrupt(interruptPin), frameIntHandler, RISING);
  hitFrame = false;
	while (!hitFrame);
  detachInterrupt(digitalPinToInterrupt(interruptPin));
}

void LiFiRXInterrupts::resetFrameInterrupt()
{
  hitFrame = false;
}
