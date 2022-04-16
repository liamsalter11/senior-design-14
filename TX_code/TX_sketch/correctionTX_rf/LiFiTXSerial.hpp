#ifndef LIFITXSERIAL
#define LIFITXSERIAL

#include "Bitset.hpp"

namespace LiFiTXSerial
{
	void writeStartUpMessage(void);
	void writeBitset(const LiFiData::Bitset&);
}

#endif