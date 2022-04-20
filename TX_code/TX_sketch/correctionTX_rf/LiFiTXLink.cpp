#include "LiFiTXLink.hpp"
#include "LiFiCorrection.hpp"

using LiFiData::Bitset;

//Makes a single byte Bitset with all 1's
Bitset getHeaderFooterBitset()
{
	Bitset hf(10);
	for (int i = 0; i < 9; i++) hf.set(i);
	return hf;
}

//Make frame number 
Bitset getNumFramesBitset(int Num)
{
  Bitset NumFrame(10);
  NumFrame.set(0);
  for (int i = 1; i < 9; i++)
  {
    if (bitRead(Num, 8-i)) NumFrame.set(i);
  }
  return NumFrame;
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
Bitset LiFiTXLink::makeTXBitsetFromString(const String& inputMessage, bool correct)
{
	Bitset tx;
	for (char c : inputMessage)
	{
		tx = tx + makeBitsetFromChar(c);
	}
	if (correct) tx = LiFiCorrection::convolve(tx);
  int NumF=tx.getLength()/8;
  Serial.print(NumF);
	tx = frameData(tx);
	tx = getHeaderFooterBitset()+ getNumFramesBitset(NumF) + tx;
	return tx;
}
