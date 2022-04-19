#ifndef LIFIRXSAMPLER
#define LIFIRXSAMPLER

#include "LiFiData::Bitset"

namespace LiFiRXSampling
{
	//Returns a frame of the given length
	LiFiData::Bitset readFrame(const int);
}

#endif