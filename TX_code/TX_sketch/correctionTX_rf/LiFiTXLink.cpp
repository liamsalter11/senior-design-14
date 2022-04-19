#include "LiFiTXLink.hpp"
#include "LiFiCorrection.hpp"

using LiFiData::Bitset;

//Makes a single byte Bitset with all 1's
Bitset getHeaderBitset()
{
	Bitset header(8);
	for (int i = 0; i < 8; i++) header.set(i);
	return header;
}

//Copies a char value into a Bitset from most significant bit down
Bitset makeBitsetFromChar(char c)
{
	Bitset letter(8);
	for (int i = 0; i < 8; i++)
	{
		if (c&(0x80>>i)) letter.set(i);
	}
	return letter;
}

Bitset makeBitsetFromString(const String& s)
{
	Bitset data;
	for (char c : s)
	{
		data = data + makeBitsetFromChar(c);
	}
	return data;
}

//Cuts a raw data Bitset into 8-bit frames with appended frame header bits
Bitset frameData(const Bitset& data)
{
	Bitset allFrames;

	for (int i = 0; i < data.getLength()/8; i++)
	{
		Bitset frame(10);
		frame.set(0);
		for (int j = 0; j < 8; j++)
		{
			if (data[j+8*i]) frame.set(j+1);
		}
		allFrames = allFrames + frame; 
	}

	return allFrames;
}

//Creates a full linked Bitset from a raw data string, will use error correction if correct = true
//TODO handle strings longer than 255 (no EC) or 127 (with EC)
Bitset LiFiTXLink::makeTXBitsetFromString(const String& inputMessage, bool correct)
{
	Bitset tx = makeBitsetFromString(inputMessage);
	if (correct) tx = LiFiCorrection::convolve(tx);
	int numFrames = (correct) ? inputMessage.length()*2 : inputMessage.length();
	tx = getHeaderBitset() + makeBitsetFromChar(numFrames) + tx;
	tx = frameData(tx);
	return tx;
}
