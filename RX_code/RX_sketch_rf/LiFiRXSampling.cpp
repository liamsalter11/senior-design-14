#include "LiFiRXSampling.hpp"

#include "LiFiRXConsts.hpp"

bool readBit()
{
	int samples = 0;
	int endTime = micros() + transRate;
	while (endTime > micros()) digitalRead(BLUE) ? sample++ : sample--;
	return sample >= 0;
}

LiFiData::Bitset LiFiRXSampling::readFrame(const int frameLength)
{
	LiFiData::Bitset frame(10);
	for (int i = 0; i < frameLength; i++)
	{
		if (readBit()) frame.set(i);
	}
}