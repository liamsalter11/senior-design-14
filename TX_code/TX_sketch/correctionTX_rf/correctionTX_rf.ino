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
	
	//Startup state for power on
	void startState()
	{
		state = 1;
	}
	
	//State for waiting for a message and sending it
	void transmitState()
	{
		String message = LiFiTXSerial::getInputMessage();
		if (message.length())
		{
			Bitset data = LiFiTXLink::makeTXBitsetFromString(message, USE_CORRECTION);
			LiFiTXSerial::writeBitset(data);
			LiFiTXController::TXOneChannel(data);
		}
		state = 2;
	}
	
	//State for resetting and preparing for next message
	void resetState()
	{
		LiFiTXController::setAllLEDs(LOW);
		LiFiTXSerial::reset();
		state = 0;
	}
	
	//Switch case to match state index to state function
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

//Run state machine continuously
void loop() 
{
	LiFiTXStateMachine::handleState();
}
