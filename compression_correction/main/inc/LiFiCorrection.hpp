#ifndef LIFICORRECTION
#define LIFICORRECTION

#include "Bitset.hpp"

namespace LiFiCorrection
{	
	LiFiData::Bitset convolve(const LiFiData::Bitset&);
	LiFiData::Bitset decodeErrors(const LiFiData::Bitset&);
};

#endif