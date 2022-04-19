#ifndef LIFIRXSAMPLER
#define LIFIRXSAMPLER

#include "Bitset.hpp"
#include "Pinset.hpp"

namespace LiFiRXSampling
{
	//Returns a frame of the given length*number of pins
	LiFiData::Bitset readFrame(const int, const LiFiData::Pinset&);
}

#endif
