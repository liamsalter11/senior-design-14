#ifndef LIFITXSERIAL
#define LIFITXSERIAL

#include "Bitset.hpp"

namespace LiFiTXSerial
{
	void initialize();
	void reset();
	void writeStartUpMessage(void);
	void writeBitset(const LiFiData::Bitset&);
	
	String getInputMessage();
}

#endif