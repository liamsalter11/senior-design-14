#include "LiFiTXController.hpp"

#include "LiFiTXConsts.hpp"

using LiFiData::Bitset;

struct Pinset
{
	int length;
	int pins[3];
};

void writeToPinset(const Pinset& pins, bool val)
{
	for (int i = 0; i < pins.length; i++) digitalWrite(pins.pins[i], val);
}

void sendFrame(const Bitset& bits, const Pinset& pins, int index)
{
	for (int i = 0; i < FRAME_SIZE; i++)
	{
		for (int j = 0; j < pins.length; j++) digitalWrite(pins.pins[j], bits[i+index+j]);
		int waitTime = 1000*TRANSMISSION_RATE + micros();
		while(micros() <= waitTime);
	}
}
	
void sendBitset(const Bitset& bits, const Pinset& pins)
{
	for (int i = 0; i < bits.getLength(); i += FRAME_SIZE)
	{
		sendFrame(bits, pins, i);
		writeToPinset(pins, LOW);
		unsigned int frameWaitTime = FRAME_DELAY * 1000 + micros();
		while(micros() <= frameWaitTime);
	}
}

void LiFiTXController::TXOneChannel(const LiFiData::Bitset& data)
{
	Serial.println("\nNow sending message");
	Pinset pins = {1, {LED_PIN_B, 0, 0}};
	sendBitset(data, pins);
	Serial.println("Bitset sent");
}

void LiFiTXController::TXThreeChannel(const LiFiData::Bitset& data)
{
	Pinset pins = {3, {LED_PIN_B, LED_PIN_R, LED_PIN_G}};
	sendBitset(data, pins);
}

void LiFiTXController::setAllLEDs(bool val)
{
	digitalWrite(LED_PIN_R, val);
	digitalWrite(LED_PIN_G, val);
	digitalWrite(LED_PIN_B, val);
	digitalWrite(LED_PIN_CLOCK, val);
}
