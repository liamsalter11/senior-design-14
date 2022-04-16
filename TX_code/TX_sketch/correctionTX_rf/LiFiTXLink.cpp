#include "LiFiTXLink.hpp"
#include "LiFiCorrection.hpp"

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

Bitset makeBitsetFromChar(char c)
{
  Bitset letter(8);
  for (int i = 0; i < 8; i++)
  {
    if (c&(0x80>>i)) letter.set(i);
  }
  return letter;
}

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

Bitset LiFiTXLink::makeTXBitsetFromString(const String& inputMessage, bool correct)
{
	Serial.println("Linking String into TX Bitset");
	Bitset data;

	data = data + getHeaderFooterBitset();
	for (char c : inputMessage)
	{
		data = data + getFrameFromLetter(c);
	}
	data = data + getHeaderFooterBitset();

	//return data;

	//Test this part
	Bitset tx;

	for (char c : inputMessage)
	{
		tx = tx + makeBitsetFromChar(c);
	}

	if (correct) tx = LiFiCorrection::convolve(tx);

	tx = frameData(tx);

	tx = getHeaderFooterBitset() + tx + getHeaderFooterBitset();
  return tx;
}
