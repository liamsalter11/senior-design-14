// a 11111111 indicates the start and end of transmission
#include <Vector.h>
#include <OneShotTimer.h>

#include "Receiver.h"



# define CLK 9
# define DIN 7
# define interruptPin 13
# define interruptControllerPin 12

# define maxSize 200
# define maxFrames 100

const int transRate = 100;
const int frameDelay = 200;
//10 bits in a frame, 0-9
const int frameSize = 10;


//variables and objects for interrupt handling
volatile int edge = 0;
OneShotTimer frameTimer(1);
OneShotTimer sampleTimer(1); 
bool frameDone = false;
bool sampleDone = false;
int samplesPerBitTime = 100;


Receiver receive(CLK, DIN);


int state = 0;
int binaryInput = 7;


//byte array to store message
byte wholeBIN[maxSize]; //replace number with max number of characters

unsigned int storageArray[maxFrames];
Vector<unsigned int> dataVector(storageArray);

unsigned int receivedFrame=0;

void setup() 
{
  //pinMode(binaryInput, INPUT);
  
  receive.setRGB(7, A3, A4);
  Serial.begin(9600);

  pinMode(interruptControllerPin, OUTPUT);
  pinMode(DIN, INPUT);
  pinMode(interruptPin, INPUT);

  digitalWrite(interruptControllerPin,HIGH);
  
  attachInterrupt(digitalPinToInterrupt(interruptPin), edgeInterrupt, RISING);
  
  //setup for the one shot timer that samples each frame
  frameTimer.start();
  frameTimer.setDurationMsec((double)transRate*frameSize);
  frameTimer.onUpdate([&]() {
        //this is what happens at the end of the timer
        frameDone = true;
    });

  //setup onshot timer that counts between each sample taken during a frame
  sampleTimer.start();
  sampleTimer.setDurationMsec((double)(transRate));
  sampleTimer.onUpdate([&]() {
        //this is what happens at the end of the timer
        sampleDone = true;
    });
}

//interrupt handler
void edgeInterrupt(void)
{

  detachInterrupt(digitalPinToInterrupt(interruptPin));
  Serial.println("edge interrupt");
  Serial.println(frameDone);
  
  int sample = 0;
  int i = 0;
  Serial.println(frameDone);
  frameTimer.update();
  while(frameDone == false)
  { 
    Serial.println("sampling frame");
    sampleTimer.update();
    while(sampleDone == false)
    {
      //Serial.println("sample bit time");
      if(digitalRead(DIN) == HIGH)
        sample++;
      else
        sample = sample -1;
    }
    //reset the conditional
    sampleDone = false;
    
    if(sample > 0)
    {
      receivedFrame<<1;
      receivedFrame | 1;
    }
    else 
    {
      receivedFrame<<0;
    }
    
    //reset the sample counter
    sample = 0;
    i++;
    //loop will be broken when the timer ends
    
  }
  //reset the conditional
  frameDone = false;
  Serial.println(receivedFrame);
  frameHandler(receivedFrame);
  Serial.println("end of interrupt ");
  //digitalWrite(interruptControllerPin, HIGH);
  attachInterrupt(digitalPinToInterrupt(interruptPin), edgeInterrupt, RISING);
}

void frameHandler(unsigned int inputFrame)
{
  //check to see if the frame head and footer have been received
  if(bitRead(inputFrame,9) == 1 && bitRead(inputFrame,0) == 0)
  {
    //get rid of frame footer 
    //0111111110 is the mask, gets rid of all start/stop bits
    int bitMask = 255;
    dataVector.push_back(inputFrame & bitMask);
  }
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

void printStartUpMessage(void)
{
  Serial.print("Welcome! We are waiting for the message to be transmitted to us.\n");
}
void printMessageToSerial(String message)
{
  Serial.print("Here is the converted message:\n");
  Serial.print(message);
  Serial.println("\n----------\n");
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
  Serial.print("\nRestarting..... \n \n");
}

void loop() 
{
  
  //Serial.print("State \n");
  switch(state)
  {
    case 0:
      printStartUpMessage();
      state=1;
      break;

    case 1:
      
      //the header of the transmission was received   
      if(dataVector.front()==255)
      {
        state=2;
      }
      else
        state=1;
      
      break;
      
    case 2:
      //the footer of the transmisson was received
      if(dataVector.back() == 255)  
        state = 3;
      else 
      {
        state = 2;
      }
      break;
    
    case 3:
      //Serial.print("staet 3");
      printMessageToSerial(convertToString(wholeBIN));
      state = 4;
    case 4:
      resetSystem();
      delay(2000);
      state=1;
      break; 

      
  }
  attachInterrupt(digitalPinToInterrupt(interruptPin), edgeInterrupt, RISING);
  

}
