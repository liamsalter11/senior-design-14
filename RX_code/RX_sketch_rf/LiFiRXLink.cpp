#include "LiFiRXLink.hpp"
#include "LiFiRXConsts.hpp"

LiFiData::Bitset unLinkFrame(const LiFiData::Bitset& frame)
{
	LiFiData::Bitset data(frame.getLength()-2);
	for (int i = 0; i < frame.getLength()-2; i++)
	{
		if (frame[i+1]) data.set(i);
	}
	return data;
}

LiFiData::Bitset getFrame(const LiFiData::Pinset& pins)
{
	detachInterrupt(digitalPinToInterrupt(interruptPin));
	LiFiData::Bitset frame = LiFiRXSampling::readFrame(frameSize, pins);
	frame = unLinkFrame(frame);
	interrupting = false;
	attachInterrupt(digitalPinToInterrupt(interruptPin), edgeInterrupt, RISING);
	return frame;
}

LiFiData::Bitset LiFiRXLink::getFrameOneChannel()
{
	return getFrame({{BLUE, 0, 0}, 1});
}

LiFiData::Bitset LiFiRXLink::getFrameTwoChannel()
{
	return getFrame({{BLUE, RED, 0}, 2});
}

LiFiData::Bitset LiFiRXLink::getFrameThreeChannel()
{
	return getFrame({{BLUE, RED, GREEN}, 3});
}