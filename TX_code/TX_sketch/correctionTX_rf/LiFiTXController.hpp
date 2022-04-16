#ifndef LIFITXCONTROLLER
#define LIFITXCONTROLLER

#include "Bitset.hpp"

namespace LiFiTXController
{
	void TXOneChannel(const LiFiData::Bitset&);
	void TXThreeChannel(const LiFiData::Bitset&);
	
	void setAllLEDs(bool);
}

#endif