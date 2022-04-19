// a 11111111 indicates the start and end of transmission
#include <Vector.h>

#include "LiFiCorrection.hpp"
#include "LiFiRXSampling.hpp"
#include "LiFiRXConsts.hpp"
#include "LiFiRXLink.hpp"

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

	attachInterrupt(digitalPinToInterrupt(interruptPin), edgeInterrupt, RISING);
}

void edgeInterrupt(void)
{
	interrupting = true;
}

int lengthBitsetToInt(const LiFiData::Bitset& bits)
{
	int length = 0;
	for (int i = 0; i < 8; i++) length += (bits.at(i)) ? (1 << (7-i)) : 0;
	return length;
}

void waitForHeaderFrame()
{
	while (!interrupting);
	LiFiData::Bitset header = LiFiRXLink::getFrameOneChannel();
	int headerVal = lengthBitsetToInt(header);
	if (headerVal != 255)
	{
		//Bad news
	}
}

int getMessageLengthFrame()
{
	while (!interrupting);
	LiFiData::Bitset messageLength = LiFiRXLink::getFrameOneChannel();
	return lengthBitsetToInt(messageLength);
}

LiFiData::Bitset readMessage(int messageLength)
{
	LiFiData::Bitset message(0);
	for (int i = 0; i < messageLength; i++)
	{
		while (!interrupting);
		message = message + LiFiRXLink::getFrameOneChannel();
	}
}

void reset()
{
	Serial.flush();
}

void loop()
{
	waitForHeaderFrame();
	int numBytes = getMessageLengthFrame();
	LiFiData::Bitset message = readMessage(numBytes);
	Serial.println(message.asString());
	Serial.println(message.asASCIIString());
	reset();
}
