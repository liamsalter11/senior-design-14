#include "LiFiTXController.hpp"

#include "LiFiTXConsts.hpp"

using LiFiData::Bitset;

//Used to group multiple GPIO pins at once
struct Pinset
{
	int length;
	int pins[3];
};

//Writes to all pins in a pinset
void writeToPinset(const Pinset& pins, bool val)
{
	for (int i = 0; i < pins.length; i++) digitalWrite(pins.pins[i], val);
}

//Delays for 1 bit time
void waitBitTime()
{
	int waitTime = 1000*TRANSMISSION_RATE + micros();
	while(micros() <= waitTime);
}

//Delays for 1 frame delay time
void waitFrameTime()
{
	int waitTime = 1000*FRAME_DELAY + micros();
	while(micros() <= waitTime);
}

void sendBit(bool val, const Pinset& pins)
{
	for (int j = 0; j < pins.length; j++) digitalWrite(pins.pins[j], val);
	waitBitTime();
}

//Sends a 10-bit frame starting from index
void sendFrame(const Bitset& bits, const Pinset& pins, int index)
{
	for (int i = 0; i < FRAME_SIZE; i++)
	{
		sendBit(bits[i+index], pins);
	}
}
	
//Sends a full Bitset 
void sendBitset(const Bitset& bits, const Pinset& pins)
{	
	for (int i = 0; i < bits.getLength(); i += FRAME_SIZE)
	{
		sendFrame(bits, pins, i);
		writeToPinset(pins, LOW);
		waitFrameTime();
	}
}

//Sends a Bitset over a single LED
void LiFiTXController::TXOneChannel(const LiFiData::Bitset& data)
{
	Serial.println("\nNow sending message");
	Pinset pins = {1, {LED_PIN_B, 0, 0}};
	sendBitset(data, pins);
	Serial.println("Bitset sent");
}

//Sends a Bitset over all 3 LEDs
void LiFiTXController::TXThreeChannel(const LiFiData::Bitset& data)
{
	Pinset pins = {3, {LED_PIN_B, LED_PIN_R, LED_PIN_G}};
	sendBitset(data, pins);
}

//Sets all LED pins to given val
void LiFiTXController::setAllLEDs(bool val)
{
	digitalWrite(LED_PIN_R, val);
	digitalWrite(LED_PIN_G, val);
	digitalWrite(LED_PIN_B, val);
	digitalWrite(LED_PIN_CLOCK, val);
}
