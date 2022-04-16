#include "LiFiCorrection.hpp"
#include "LiFiTXController.hpp"
#include "LiFiTXSerial.hpp"
#include "LiFiTXConsts.hpp"
#include "LiFiTXLink.hpp"

using LiFiData::Bitset;

namespace LiFiTXStateMachine
{
	namespace
	{
		int state = 0;
	}
	
	void startState()
	{
		state = 1;
	}
	
	void transmitState()
	{
		String message = LiFiTXSerial::getInputMessage();
		if (message.length())
		{
			Bitset data = LiFiTXLink::makeTXBitsetFromString(message);
			LiFiTXSerial::writeBitset(data);
			LiFiTXController::TXOneChannel(data);
		}
		state = 2;
	}
	
	void resetState()
	{
		LiFiTXController::setAllLEDs(LOW);	
		LiFiTXSerial::reset();
		state = 0;
	}
	
	void handleState()
	{
		switch(state)
		{
			case 0:
				startState();
				break;
			case 1:
				transmitState();
				break;
			case 2:
				resetState();
				break;
			default:
				state = 2;
		}
	}
}

void setup() 
{
	LiFiTXSerial::initialize();

	pinMode(LED_PIN_R, OUTPUT);
	pinMode(LED_PIN_G, OUTPUT);
	pinMode(LED_PIN_B, OUTPUT);
	pinMode(LED_PIN_CLOCK, OUTPUT);

	LiFiTXController::setAllLEDs(LOW);
}

void loop() 
{
	LiFiTXStateMachine::handleState();
}
