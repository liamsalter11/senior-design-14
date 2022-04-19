// a 11111111 indicates the start and end of transmission
#include <Vector.h>

#include "LiFiCorrection.hpp"
#include "LiFiRXSampling.hpp"

using namespace LiFiData;

/***MOVE TO CONSTS***/
#include "LiFiRXConsts.hpp"

/***REMOVE GLOBAL VARS***/
int edge = 0;
volatile bool frameDone = false;
volatile bool sampleDone = false;
volatile bool interrupting = false;
volatile int bitsReceived = 0;
volatile int state = 0;
int frameStartTime = 0;
bool frameStartLock = false;
bool gotNumFrames = false;
unsigned int storageArray[maxFrames*2];
Vector<unsigned int> dataVector(storageArray);
LiFiData::Bitset receivedBits(MAX_BITSET_LENGTH);
unsigned int numFrames = 0;
unsigned int numFramesReceived = 0;
unsigned int receivedFrame=0;

LiFiData::Bitset unLinkFrame(const LiFiData::Bitset& frame)
{
	LiFiData::Bitset data(frame.getLength()-2);
	for (int i = 0; i < frame.getLength()-2; i++)
	{
		if (frame[i+1]) data.set(i);
	}
	return data;
}

LiFiData::Bitset getFrame(const LiFiData::Pinset& pins)
{
	detachInterrupt(digitalPinToInterrupt(interruptPin));
	LiFiData::Bitset frame = LiFiRXSampling::readFrame(frameSize, pins);
	frame = unLinkFrame(frame);
	interrupting = false; //???
	attachInterrupt(digitalPinToInterrupt(interruptPin), edgeInterrupt, RISING);
	return frame;
}

LiFiData::Bitset getFrameOneChannel()
{
	return getFrame({{BLUE, 0, 0}, 1});
}

LiFiData::Bitset getFrameRB()
{
	return getFrame({{BLUE, RED, 0}, 2});
}

LiFiData::Bitset getFrameRGB()
{
	return getFrame({{BLUE, RED, GREEN}, 3});
}

/***MOVE TO SERIAL***/

void printStartUpMessage(void) { }


void printDataVector()
{
  Serial.println("");
  for(int i =0; i<dataVector.size()-1;i++)
  {
    Serial.print((char)dataVector.at(i));
  }
}

void printBitSet()
{
  dataVector.remove(0);
  dataVector.pop_back();

  receivedBits.setLength(dataVector.size()*8);
  
  for(int i=0; i<dataVector.at(i);i++)
  {
    for(int j=0; j<8;j++)
    {
      if(bitRead(dataVector.at(i), 7-j)==1)
      {
        receivedBits.set((i*8)+j);
      }
    }
  }
  receivedBits = LiFiCorrection::decodeErrors(receivedBits);
  Serial.println(receivedBits.asASCIIString()); 
}

/***QUARANTINE ZONE***/

/*
*	Functions/modules:
		Read bit
		Wait for frame
*/


void resetSystem(void)
{
  Serial.flush();
  frameStartLock = false;
  gotNumFrames = false;
  dataVector.clear();
  
  for(int i =0;i<MAX_BITSET_LENGTH;i++)
    receivedBits.clear(i);
}

/******/

void setup() 
{
  Serial.begin(19200);
  
  pinMode(interruptControllerPin, OUTPUT);
  pinMode(BLUE, INPUT);
  pinMode(GREEN, INPUT);
  pinMode(RED,INPUT);
  pinMode(interruptPin, INPUT);

  digitalWrite(interruptControllerPin,HIGH);
  
  attachInterrupt(digitalPinToInterrupt(interruptPin), edgeInterrupt, RISING);
}

void edgeInterrupt(void)
{
  interrupting = true;
  state = 5;
}

/***MAKE STATE MACHINE/SIMPLIFY***/

void loop() 
{
  switch(state)
  {
    case 0:
      printStartUpMessage();
      if(!interrupting)
        state=6;
      break;

    case 1:
      if(dataVector.front()==255)
      {
        frameStartLock = true;
        if(!interrupting)  
          state=7;
      }
      else
      {
       if(!interrupting)   
          state=1;
      }
      
      break;
      
    case 2:
      if(numFramesReceived >= numFrames)
      {  
          state = 3;
      }
      else 
      {
        if(!interrupting)
          state = 2;
      }
      break;
    
    case 3:
      printDataVector();
      
      if(!interrupting)
        state = 4;
        
    case 4:
      resetSystem();
      state=1;
      break;
    case 5:
      getFrame();
      
      if(gotNumFrames == true)
      {
        numFramesReceived++;
      }
       
      if(!frameStartLock)
      {
        frameStartLock = true;
        state = 1;
      }
      else if(!gotNumFrames)
        state = 7;
      else
        state = 2;
      
      break; 
      
    case 6:
      state=6;
      break;

    case 7:
      if(dataVector.size()==2 && !gotNumFrames)
      {
        numFrames = dataVector.at(1);
        gotNumFrames = true;
        if(!interrupting)
          state = 2;
      }
      else
      { 
        if(!interrupting)
          state = 7;
      }
      break;
      
  }
}
