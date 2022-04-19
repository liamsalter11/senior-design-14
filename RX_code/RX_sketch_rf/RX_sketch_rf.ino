// a 11111111 indicates the start and end of transmission
#include <Vector.h>

#include "LiFiCorrection.hpp"

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

bool readBit()
{
	int samples = 0;
	int endTime = micros() + transRate;
	while (endTime > micros()) digitalRead(BLUE) ? sample++ : sample--;
	return sample >= 0;
}

LiFiData::Bitset readFrame()
{
	LiFiData::Bitset frame(10);
	for (int i = 0; i < frameSize; i++)
	{
		if (readBit()) frame.set(i);
	}
}

//These 3 functions appear to do almost the same thing
void getFrame(void)
{
  detachInterrupt(digitalPinToInterrupt(interruptPin));
  
  receivedFrame = 0;
  frameDone = false;
  sampleDone = false; 
  volatile int sample = 0;	//Why volatile???
  int startTime = 0;
  while(!frameDone)
  { 
    startTime = micros();
    while(!sampleDone && !frameDone) 
    {
      digitalRead(BLUE) ? sample++ : sample--;

      int t = transRate + startTime;	//Shouldn't calculate this every loop
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

void getFrameRB(void)
{
  detachInterrupt(digitalPinToInterrupt(interruptPin));
  
  unsigned int receivedFrameR = 0;
  unsigned int receivedFrameB = 0;
  frameDone = false;
  sampleDone = false; 
  volatile int sampleB = 0;
  volatile int sampleR = 0;
  int startTime = 0;
  while(!frameDone)
  { 
    startTime = micros();
    while(!sampleDone && !frameDone) 
    {
      digitalRead(BLUE) ? sampleB++ : sampleB--;
      digitalRead(RED) ? sampleR++ : sampleR--;

      int t = transRate + startTime;
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
    receivedFrameB = receivedFrameB<<1;
    if(sampleB > 0)
      receivedFrameB++;

    receivedFrameR = receivedFrameR<<1;
    if(sampleR > 0)
      receivedFrameR++;
    sampleR = 0;
    sampleB = 0;
  }
  frameHandler(receivedFrameB);
  frameHandler(receivedFrameR);

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
    
    inputFrame = inputFrame & bitMask;
    dataVector.push_back(inputFrame);
}


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
