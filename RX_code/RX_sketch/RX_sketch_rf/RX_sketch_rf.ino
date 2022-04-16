// a 11111111 indicates the start and end of transmission
#include <Vector.h>

#include "Receiver.h"

# define BLUE 7
# define GREEN 9
# define RED 10
# define CLK 9
# define DIN 7
# define interruptPin 13
# define interruptControllerPin 12

# define maxSize 200
# define maxFrames 100

const int transRate = 1; //ms
//const int frameDelay = 300; //ms
//10 bits in a frame, 0-9
const int frameSize = 10;

//variables and objects for interrupt handling
int edge = 0;
volatile bool frameDone = false;
volatile bool sampleDone = false;
volatile bool interrupting = false;
volatile int bitsReceived = 0;

Receiver receive(CLK, DIN);

volatile int state = 0;
int frameStartTime = 0;
bool frameStartLock = false;
int frameTimeOut = 10000000;

byte wholeBIN[maxSize]; //replace number with max number of characters

unsigned int storageArray[maxFrames];
Vector<unsigned int> dataVector(storageArray);

unsigned int receivedFrame=0;

/***Serial Print Functions***/

void printStartUpMessage(void)
{
}

void printMessageToSerial(String message)
{
  Serial.print("Here is the converted message:\n");
  Serial.print(message);
  Serial.println("\n----------\n");
}

void printDataVector()
{
  Serial.println("");
  for(int i =0; i<dataVector.size()-1;i++)
  {
    Serial.print((char)dataVector.at(i));
  }
}

/***Do Stuff Functions***/

void setup() 
{
  receive.setRGB(7, A3, A4);
  Serial.begin(19200);
  
  pinMode(interruptControllerPin, OUTPUT);
  pinMode(DIN, INPUT);
  pinMode(interruptPin, INPUT);

  digitalWrite(interruptControllerPin,HIGH);
  
  attachInterrupt(digitalPinToInterrupt(interruptPin), edgeInterrupt, RISING);
}

//interrupt handler

void edgeInterrupt(void)
{
  interrupting = true;
  state = 5;
}

void getFrame(void)
{
  detachInterrupt(digitalPinToInterrupt(interruptPin));
  
  receivedFrame = 0;
  frameDone = false;
  sampleDone = false; 
  volatile int sample = 0;
  int startTime = 0;
  while(!frameDone)
  { 
    startTime = micros();
    while(!sampleDone && !frameDone) 
    {
      digitalRead(DIN) ? sample++ : sample--;

      int t = transRate * 1000 + startTime;
      if(micros() >= t)
      {
        sampleDone = true;
        bitsReceived++;
        if(bitsReceived == frameSize)
        {
          frameDone = true;
          bitsReceived = 0;
        }
      }

    }    
    sampleDone = false;
    receivedFrame = receivedFrame<<1;
    if(sample > 0)
      receivedFrame++;

    sample = 0;       
  }
  frameHandler(receivedFrame);
  interrupting = false;
  attachInterrupt(digitalPinToInterrupt(interruptPin), edgeInterrupt, RISING); 
}

void getFrameRGB(void)
{
  detachInterrupt(digitalPinToInterrupt(interruptPin));
  
  receivedFrame = 0;
  frameDone = false;
  sampleDone = false; 
  volatile int sampleR = 0, sampleG = 0, sampleB = 0;
  int startTime = 0;
  while(!frameDone)
  {
    startTime = micros();
    while(!sampleDone && !frameDone) 
    {
      digitalRead(GREEN) ? sampleG++ : sampleG--;
      digitalRead(BLUE) ? sampleB++ : sampleB--;
      digitalRead(RED) ? sampleR++ : sampleR--;

      int t = transRate * 1000 + startTime;
      if(micros() >= t)
      {
        sampleDone = true;
        bitsReceived++;
        if(bitsReceived == frameSize)
        {
          frameDone = true;
          bitsReceived = 0;
        }
      }

    }
    sampleDone = false;

    receivedFrame = receivedFrame << 3;
    receivedFrame |= ((sampleB > 0) << 2) | ((sampleG > 0) << 1) | (sampleR > 0);

    sampleR = 0;
    sampleG = 0;
    sampleB = 0; 
  }
  
  frameHandler(receivedFrame);

  interrupting = false;
  attachInterrupt(digitalPinToInterrupt(interruptPin), edgeInterrupt, RISING); 
}

void frameHandler(unsigned int inputFrame)
{
    inputFrame = inputFrame>>1;
    uint8_t bitMask = 255;
    dataVector.push_back(inputFrame & bitMask);
}

String convertToString(byte bytes[maxSize])
{
  String convertedMessage=""; 
  for(int i=0; i<=maxSize;i++)
  {
     if(bytes[i] ==13)
      break;
     convertedMessage.concat((char)dataVector.at(i));
  }
  
  return convertedMessage;
}

void getInputBinary(void)
{
  byte input;
  input = (byte)receive.ReadByte();
  dataVector.push_back(input);
}

void resetSystem(void)
{
  for(int i = 0; i<sizeof(wholeBIN);i++)
  {
    wholeBIN[i] = 0;
    
  }
  Serial.flush();
  frameStartLock = false;
  dataVector.clear();
}

namespace LiFiRXStateMachine
{
	/*
	namespace
	{
		int state;
	}
	*/

	void startState()
	{
		printStartUpMessage();
		if(!interrupting)
		state = 6;
	}
	
	void state1()
	{
		if(dataVector.front()==255)
		{
			frameStartLock = true;
			if(!interrupting)  
			state = 2;
		}
		else
		{
			if(!interrupting) state = 1;
		}
	}
	
	void state2()
	{
		if (dataVector.back() == 255 && dataVector.size()>1)
		{  
			if(!interrupting) state = 3;
		}
		else 
		{
			if(!interrupting) state = 2;
		}
	}
	
	void state3()
	{
		printDataVector();
		if(!interrupting) state = 4;
	}
	
	void state4()
	{
		resetSystem();
		state = 1;
	}
	
	void state5()
	{
		getFrame();

		if(!frameStartLock)
		{
			frameStartLock = true;
			state = 1;
		}
		else state = 2;
	}
	
	void state6()
	{
		state = 6;
	}
}

void loop() 
{
	switch(state)
	{
		case 0:	
			LiFiRXStateMachine::startState();
			break;

		case 1:
			LiFiRXStateMachine::state1();      
			break;

		case 2:
			LiFiRXStateMachine::state2();
			break;

		case 3:
			LiFiRXStateMachine::state3();
			break;
			
		case 4:
			LiFiRXStateMachine::state4();
			break;
			
		case 5:
			LiFiRXStateMachine::state5();
			break;
			
		case 6:
			LiFiRXStateMachine::state6();
			break;  
	}
}
