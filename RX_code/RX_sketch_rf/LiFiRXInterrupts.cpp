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
}

void LiFiRXInterrupts::disableFrameInterrupt()
{
	detachInterrupt(digitalPinToInterrupt(interruptPin));
}

void LiFiRXInterrupts::waitForFrameInterrupt()
{
	while (!hitFrame);
	hitFrame = false;
}