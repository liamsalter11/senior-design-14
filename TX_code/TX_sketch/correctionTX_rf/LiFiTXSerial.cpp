#include "LiFiTXSerial.hpp"

void LiFiTXSerial::initialize()
{
	Serial.begin(19200);
	Serial.print("Starting\n");
}

void LiFiTXSerial::reset()
{
	Serial.flush();
	Serial.print("\nRestarting..... \n \n");
}

void LiFiTXSerial::writeStartUpMessage()
{
	Serial.println("Hello! You can input a message below and have it turned into binary!");
	Serial.println("Enter Message: "); 
}

void LiFiTXSerial::writeBitset(const LiFiData::Bitset& data)
{
	Serial.println("----------");
	Serial.print("Message is:\n");
	Serial.print(data.getLength());
	for(int i = 0; i<data.getLength();i++)
	{
		Serial.print(data[i], BIN);
	}
}