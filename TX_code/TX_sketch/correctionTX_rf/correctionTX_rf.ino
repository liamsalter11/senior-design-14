#include <Vector.h>

#include "LiFiCorrection.hpp"
#include "LiFiTXController.hpp"
#include "LiFiTXSerial.hpp"

using LiFiData::Bitset;

//vars for getting string
String userString;
byte charByte[7]; //array for converting char into bin
byte charByteSum=0; //array for store the final value of a char's bin
byte wholeBIN[MAX_SIZE]; //replace the number with the max number of characters

unsigned int storageArray[MAX_FRAMES];
Vector<unsigned int> frames(storageArray);

//Bitset for correction
LiFiData::Bitset convolBits(LiFiData::MAX_BITSET_LENGTH);

//loop vars 
int state;
char line[100];
int count = 0;
char ledMessage[7];

/***Doing Stuff Functions***/

void setup() {
	LiFiTXSerial::initialize();

	pinMode(LED_PIN_R, OUTPUT);
	pinMode(LED_PIN_G, OUTPUT);
	pinMode(LED_PIN_B, OUTPUT);
	pinMode(LED_PIN_CLOCK, OUTPUT);

	digitalWrite(LED_PIN_R, LOW);
	digitalWrite(LED_PIN_G, LOW);
	digitalWrite(LED_PIN_B, LOW);
	digitalWrite(LED_PIN_CLOCK, LOW);
}

void convertString2Binary(String inputMessage)
{
  addTransmissionStartEnd();
  int messageLength = inputMessage.length();
  char inputChars[messageLength]; 
  char inByte;
  int inputPosition=0;
  
  char letter;
  int bytes[messageLength];
    
  for(int i=0; i<messageLength-1; i++)
  {
    letter = inputMessage.charAt(i);
    wholeBIN[i] = inputMessage.charAt(i);
    frames.push_back(generateFrame(letter));  
  }
 
  LiFiData::Bitset rawData(frames.size()*10);
  for(int i =0; i<frames.size();i++)
  {
    for(int j =0; j<9; j++)
     {
        if(bitRead(frames.at(i),j) == 1)
        {
          rawData.set((10*i)+j);
        }
     }
  }
  convolBits = LiFiCorrection::convolve(rawData);  
  addTransmissionStartEnd();
}

bool getInputMessage(void)
{
  if (Serial.available() > 0) 
  {
  
    line[count] = (char)Serial.read(); // store the char
    if (line[count++] == '\n'){        // if its a CR,
       line[count] = '\0';             //  zero-terminate it
       Serial.println("----------\nYour message is: ");
       Serial.println(line);           //  and print it.
       userString = line;
       if(userString.length()>1)  
         return(true);
         
       Serial.println("loop");         // re-prompt
       count = 0;                      //  and reset the index.
    }
  }

  return(false);
}

void resetSystem(void)
{
	userString="";
	line[0]='\0';
	count=0;
	for(unsigned int i = 0; i < MAX_SIZE; i++)
	{
		wholeBIN[i] = 0;
	}

	frames.clear();
	digitalWrite(LED_PIN_R, LOW);
	digitalWrite(LED_PIN_G, LOW);
	digitalWrite(LED_PIN_B, LOW);
	digitalWrite(LED_PIN_CLOCK, LOW);
	Serial.flush();
	Serial.print("\nRestarting..... \n \n");
}


void addTransmissionStartEnd(void)
{
    frames.push_back(generateFrame(255));
    Serial.println("adding header/footer");
}

unsigned int generateFrame(byte charByte)
{
	unsigned int frame = 1;
	frame = frame << 8;
	frame |= charByte;
	frame = frame << 1;
	return frame;
}

void printFrames(void)
{
	LiFiData::Bitset data(frames.data(), frames.size());
	LiFiTXController::TXOneChannel(data);
}

void sendConvolBits(void)
{
	LiFiTXController::TXOneChannel(convolBits);
}

void printFramesRGB(void)
{
	LiFiData::Bitset data(frames.data(), frames.size());
	LiFiTXController::TXThreeChannel(data);
}

namespace LiFiTXStateMachine
{
	namespace
	{
		int state = 1;
	}
	
	void startState()
	{
		state = 1;
	}
	
	void transmitState()
	{
		if(getInputMessage())
		{
			convertString2Binary(userString);
			sendConvolBits();
			LiFiTXSerial::writeBitset(LiFiData::Bitset(frames.data(), frames.size()));
			LiFiTXSerial::writeBitset(convolBits);
			state = 2;
			return;
		}
		state = 1;
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
