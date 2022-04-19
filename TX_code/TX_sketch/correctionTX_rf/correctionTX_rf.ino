#include "LiFiCorrection.hpp"
#include "LiFiTXController.hpp"
#include "LiFiTXSerial.hpp"
#include "LiFiTXConsts.hpp"
#include "LiFiTXLink.hpp"

//Initialize all GPIO pins
void setup() 
{
	LiFiTXSerial::initialize();

	pinMode(LED_PIN_R, OUTPUT);
	pinMode(LED_PIN_G, OUTPUT);
	pinMode(LED_PIN_B, OUTPUT);
	pinMode(LED_PIN_CLOCK, OUTPUT);

	LiFiTXController::setAllLEDs(LOW);
}

String getNextMessage()
{
	return LiFiTXSerial::getInputMessage();
}

void linkMessage(String& message)
{
	LiFiData::Bitset data;
	if (message.length()) LiFiData::Bitset data = LiFiTXLink::makeTXBitsetFromString(message, USE_CORRECTION);
	return data;
}

void transmitMessage(Bitset& data)
{
	LiFiTXSerial::writeBitset(data);
	LiFiTXController::TXOneChannel(data);
}

void resetTX()
{
	LiFiTXController::setAllLEDs(LOW);
	LiFiTXSerial::reset();
}

//Run state machine continuously
void loop() 
{
	String message = getNextMessage();
	LiFiData::Bitset data = linkMessage(message);
	transmitMessage(data);
	resetTX();
}
