// a 11111111 indicates the start and end of transmission
#include "LiFiCorrection.hpp"
#include "LiFiRXSampling.hpp"
#include "LiFiRXConsts.hpp"
#include "LiFiRXLink.hpp"
#include "LiFiCorrection.hpp"

volatile bool interrupting = false;

void setup() 
{
	Serial.begin(19200);

	pinMode(interruptControllerPin, OUTPUT);
	pinMode(BLUE, INPUT);
	pinMode(GREEN, INPUT);
	pinMode(RED, INPUT);
	pinMode(interruptPin, INPUT);

	digitalWrite(interruptControllerPin,HIGH);
}

int lengthBitsetToInt(const LiFiData::Bitset& bits)
{
	int length = 0;
	for (int i = 0; i < 8; i++) length += (bits.at(i)) ? (1 << (7-i)) : 0;
	return length;
}

bool waitForHeaderFrame()
{
	LiFiData::Bitset header = LiFiRXLink::getFrameOneChannel();
	int headerVal = lengthBitsetToInt(header);
	if (headerVal != 255)
	{
		return false;
	}
  return true;
}

int getMessageLengthFrame()
{
	LiFiData::Bitset messageLength = LiFiRXLink::getFrameOneChannel();
	return lengthBitsetToInt(messageLength);
}

LiFiData::Bitset readMessage(int messageLength)
{
	LiFiData::Bitset message(0);
	for (int i = 0; i < messageLength; i++)
	{
	  message = message + LiFiRXLink::getFrameOneChannel();
	}
  return message;
}

void reset()
{
	Serial.flush();
}

void loop()
{
	while (!waitForHeaderFrame());
	int numBytes = getMessageLengthFrame();
	LiFiData::Bitset message = readMessage(numBytes);
  if (USE_CORRECTION) message = LiFiCorrection::decodeErrors(message);
	Serial.println(message.asASCIIString());
	reset();
}
