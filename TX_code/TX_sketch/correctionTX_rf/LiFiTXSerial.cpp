#include "LiFiTXSerial.hpp"

//Initializes Serial communication
void LiFiTXSerial::initialize()
{
	Serial.begin(19200);
	Serial.print("Starting\n");
}

//Resets Serial communication
void LiFiTXSerial::reset()
{
	Serial.flush();
	Serial.print("\nRestarting..... \n \n");
}

//Prints start up message to user -> not currently in use
void LiFiTXSerial::writeStartUpMessage()
{
	Serial.println("Hello! You can input a message below and have it turned into binary!");
	Serial.println("Enter Message: "); 
}

//Prints a Bitset to Serial as a binary string
void LiFiTXSerial::writeBitset(const LiFiData::Bitset& data)
{
	Serial.println("----------");
	Serial.print("Message is:\n");
	Serial.print("Length: ");
	Serial.print(data.getLength());
	Serial.print("\n");
	for(int i = 0; i<data.getLength();i++)
	{
		Serial.print(data[i], BIN);
	}
}

//Reads a line from Serial communication into a String
String LiFiTXSerial::getInputMessage(void)
{
	String message = "";
	Serial.flush();
	while (!Serial.available());
	char letter = Serial.read();
	while (letter != '\r' && letter != '\n') 
	{
		Serial.println(letter, HEX);
		message += letter;
		while (!Serial.available());
		letter = Serial.read();
	}
	Serial.println("----------\nYour message is: ");
	Serial.println(message);
	return message; 
}
