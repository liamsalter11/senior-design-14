#include "LiFiTXLink.hpp"

using LiFiData::Bitset;

Bitset getHeaderFooterBitset()
{
	Bitset hf(8);
	for (int i = 0; i < 8; i++) hf.set(i);
	return hf;
}

Bitset getFrameFromLetter(char c)
{
	Bitset frame(10);
	frame.set(0);
	for (int i = 0; i < 8; i++)
	{
		if (c&(0x80>>i)) frame.set(i+1);
	}
	return frame;
}

Bitset LiFiTXLink::makeTXBitsetFromString(String inputMessage)
{
  Serial.println("Linking String into TX Bitset");
	Bitset data(0);
	
	data = data + getHeaderFooterBitset();
	for (char c : inputMessage)
	{
		data = data + getFrameFromLetter(c);
	}
	data = data + getHeaderFooterBitset();
	
	return data;
}
