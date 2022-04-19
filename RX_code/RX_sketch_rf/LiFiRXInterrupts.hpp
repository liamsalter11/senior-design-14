#ifndef LIFIRXINTERRUPTS
#define LIFIRXINTERRUPTS

namespace LiFiRXInterrupts
{
	void enableFrameInterrupt();
	void disableFrameInterrupt();
	void waitForFrameInterrupt();
  void resetFrameInterrupt();
}

#endif
