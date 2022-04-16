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

//byte array to store message
byte wholeBIN[maxSize]; //replace number with max number of characters

unsigned int storageArray[maxFrames];
Vector<unsigned int> dataVector(storageArray);

unsigned int receivedFrame=0;

/***Serial Print Functions***/

void printStartUpMessage(void)
{
  //Serial.print("Welcome! We are waiting for the message to be transmitted to us.\n");
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
  //  Serial.print(" ");
  //  Serial.print(dataVector.at(i));
  //Serial.println();
  }
}

/***Do Stuff Functions***/

void setup() 
{
  //pinMode(binaryInput, INPUT);
  
  receive.setRGB(7, A3, A4);
  Serial.begin(19200);
  
  pinMode(interruptControllerPin, OUTPUT);
  pinMode(DIN, INPUT);
  pinMode(interruptPin, INPUT);

  digitalWrite(interruptControllerPin,HIGH);
  
  attachInterrupt(digitalPinToInterrupt(interruptPin), edgeInterrupt, RISING);

  //setup onshot timer that counts between each sample taken during a frame
  //sampleTimer.setDurationMsec((double)(transRate));
}

//interrupt handler

void edgeInterrupt(void)
{
  interrupting = true;
  state = 5;
  //Serial.println("trigger");
}

void getFrame(void)
{
  //detach the interrupt so that it doesnt trigger inside of a frame
  detachInterrupt(digitalPinToInterrupt(interruptPin));
  //noInterrupts();
  //Serial.println("edge interrupt");
  //Serial.println(micros());
  
  receivedFrame = 0;
  frameDone = false;
  sampleDone = false; 
  volatile int sample = 0;
  int startTime = 0;
  while(!frameDone)
  { 
    
    //Serial.println(frameDone);
    
    //trigger the sampling timer, if the timer is over then restart the timer, restart the timer for the next frame
    /*if(sampleTimer.isRunning() == 0)
    {
      sampleTimer.restart();
    }
    sampleTimer.pause();
    sampleTimer.start();*/
    //Serial.print(sampleDone);
    //Serial.print(frameDone);
    startTime = micros();
    while(!sampleDone && !frameDone) 
    {
      
      //Serial.print(" ");
      //Serial.print(sample);
      digitalRead(DIN) ? sample++ : sample--;
      //sampleTimer.update();

      int t = transRate * 1000 + startTime;
      if(micros() >= t)
      {
        sampleDone = true;
        bitsReceived++;
        if(bitsReceived == frameSize)
        {
          frameDone = true;
          //Serial.println("frameDone");
          bitsReceived = 0;
        }
      }

    }    
    //reset the conditional
    sampleDone = false;
    //Serial.println(sample);
    receivedFrame = receivedFrame<<1;
    if(sample > 0)
      receivedFrame++;
  
    //reset the sample counter
    sample = 0;       
  }
  
  //Serial.println(receivedFrame, BIN);
  frameHandler(receivedFrame);
  //Serial.println("end of interrupt ");
  //digitalWrite(interruptControllerPin, HIGH);
  //Serial.println(micros());
  //reset the conditional
  //frameDone = false;
  interrupting = false;
  attachInterrupt(digitalPinToInterrupt(interruptPin), edgeInterrupt, RISING);
  //interrupts();  
}

void getFrameRGB(void)
{
  //detach the interrupt so that it doesnt trigger inside of a frame
  detachInterrupt(digitalPinToInterrupt(interruptPin));
  
  receivedFrame = 0;
  frameDone = false;
  sampleDone = false; 
  volatile int sampleR = 0, sampleG = 0, sampleB = 0;
  int startTime = 0;
  while(!frameDone)
  {
    //trigger the sampling timer, if the timer is over then restart the timer, restart the timer for the next frame
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
      
    //reset the conditional
    sampleDone = false;

    receivedFrame = receivedFrame << 3;
    receivedFrame |= ((sampleB > 0) << 2) | ((sampleG > 0) << 1) | (sampleR > 0);
  
    //reset the sample counter
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
  //Serial.println(inputFrame, BIN);
 
    //get rid of frame footer 
    //0111111110 is the mask, gets rid of all start/stop bits
    inputFrame = inputFrame>>1;
    uint8_t bitMask = 255;
    dataVector.push_back(inputFrame & bitMask);
    //Serial.println((char)(inputFrame & bitMask));
}

String convertToString(byte bytes[maxSize])
{
  String convertedMessage=""; 
  //Serial.println(sizeof(bytes));
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
      
  
  //for(int i=0; i<sizeof(wholeBIN); i++)
  //{  
  //Serial.print(wholeBIN[i]);
  //}
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
  //Serial.println("resettging daatat");
  //printDataVector();
  
  //Serial.print("\nRestarting..... \n \n");
}

void loop() 
{
  
  //Serial.print("State \n");
  switch(state)
  {
    case 0:
      printStartUpMessage();
      if(!interrupting)
        state=6;
      break;

    case 1:
      
      //the header of the transmission was received  
      //Serial.print("header State"); 
      if(dataVector.front()==255)
      {
        //Serial.println("header received");
        frameStartLock = true;
        if(!interrupting)  
          state=2;
      }
      else
      {
       if(!interrupting)   
          state=1;
      }
      
      break;
      
    case 2:
      //the footer of the transmisson was received
      //Serial.print("2");
      
      if(dataVector.back() == 255 && dataVector.size()>1)
      {  
        //Serial.println("footer received");
        if(!interrupting)
          state = 3;
      }
      else 
      {
        if(!interrupting)
          state = 2;
      }
      break;
    
    case 3:
      //Serial.print("staet 3");
      //printMessageToSerial(convertToString(wholeBIN));
      printDataVector();
      if(!interrupting)
        state = 4;
    case 4:
      resetSystem();
      //delay(2000);
      state=1;
      break;
    case 5:
      getFrame();
      
      if(!frameStartLock)
      {
        //Serial.println("locking");
        frameStartLock = true;
        state = 1;
      }
      else
        state=2;
      
      break; 
    case 6:
      state=6;
      break;

      
  }
  /*if(frameDone == true)
    attachInterrupt(digitalPinToInterrupt(interruptPin), edgeInterrupt, RISING);*/


}
