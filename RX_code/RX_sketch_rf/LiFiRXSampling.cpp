#include "LiFiRXSampling.hpp"
#include "LiFiRXConsts.hpp"

/*
 	Multi channel frames
	Bitset is ordered received bits in the following order:
		<- Channel 1 - Channel 2 - Channel 3 ->
	Interleaving channel bits
	Pinset
		pins[0] - Channel 1
		pins[1] - Channel 2
		pins[2] - Channel 3
		
	I suspect this isn't the way this was done before but the TX doesn't handle multi channel right now as well anyway
*/

int readBit(const LiFiData::Pinset& pins)
{
	int samp0 = 0, samp1 = 0, samp2 = 0;
	int endTime = micros() + transRate;
	while (endTime > micros())
	{
		if (pins.pins[0]) digitalRead(pins.pins[0]) ? samp0++ : samp0--;
		if (pins.pins[1]) digitalRead(pins.pins[1]) ? samp1++ : samp1--;
		if (pins.pins[2]) digitalRead(pins.pins[2]) ? samp2++ : samp2--;
	}
	int result = 4*(samp0>0) + 2*(samp1>0) + (samp2>0);
	return samp0>0;
}

LiFiData::Bitset LiFiRXSampling::readFrame(const int frameLength, const LiFiData::Pinset& pins)
{
	LiFiData::Bitset frame(frameLength*pins.num);
	if (!readBit()) return frame;
	frame.set(0);
	for (int i = 1; i < frameLength*pins.num; i += pins.num)
	{
		int bits = readBit(pins);
		//if (bits&1) frame.set(i+2);
		//if (bits&2) frame.set(i+1);
		if (bits) frame.set(i);
	}
	return frame;
}
