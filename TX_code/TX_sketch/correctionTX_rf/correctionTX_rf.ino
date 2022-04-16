#include <Vector.h>

#include "LiFiCorrection.hpp"
#include "LiFiTXController.hpp"
#include "LiFiTXSerial.hpp"
#include "LiFiTXConsts.hpp"

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
	for (int i = 1; i < 9; i++)
	{
		if (bitRead(c, i-1)) frame.set(i);
	}
	return frame;
}

Bitset makeTXBitsetFromString(String inputMessage)
{
	Bitset data(0);
	
	data = data + getHeaderFooterBitset();
	for (char c : inputMessage)
	{
		data = data + getFrameFromLetter(c);
	}
	data = data + getHeaderFooterBitset();
	
	return data;
}

String getInputMessage(void)
{
	String message = "";
	while (Serial.available()) 
	{
		char letter = Serial.read();
		message += letter;
		if (letter == '\n')
		{
			Serial.println("----------\nYour message is: ");
			Serial.println(message); 
		}
	}
	return message;
}

void setAllLEDs(bool val)
{
	digitalWrite(LED_PIN_R, val);
	digitalWrite(LED_PIN_G, val);
	digitalWrite(LED_PIN_B, val);
	digitalWrite(LED_PIN_CLOCK, val);
}

void setup() {
	LiFiTXSerial::initialize();

	pinMode(LED_PIN_R, OUTPUT);
	pinMode(LED_PIN_G, OUTPUT);
	pinMode(LED_PIN_B, OUTPUT);
	pinMode(LED_PIN_CLOCK, OUTPUT);

	setAllLEDs(LOW);
}

void resetSystem(void)
{
	setAllLEDs(LOW);	
	LiFiTXSerial::reset();
}

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
		String message = getInputMessage();
		if (message.length())
		{
			Bitset data = makeTXBitsetFromString(message);
			LiFiTXSerial::writeBitset(data);
			LiFiTXController::TXOneChannel(data);
		}
		state = 2;
	}
	
	void resetState()
	{
		resetSystem();
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


void loop() 
{
	LiFiTXStateMachine::handleState();
}
