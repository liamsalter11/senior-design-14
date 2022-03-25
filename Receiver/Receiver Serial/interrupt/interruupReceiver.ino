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
OneShotTimer sampleTimer(transRate/1000); 
volatile bool frameDone = false;
volatile bool sampleDone = false;
volatile int bitsReceived = 0;

Receiver receive(CLK, DIN);


int state = 0;
int binaryInput = 7;


//byte array to store message
byte wholeBIN[maxSize]; //replace number with max number of characters

unsigned int storageArray[maxFrames];
Vector<unsigned int> dataVector(storageArray);

volatile unsigned int receivedFrame=0;

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

  //setup onshot timer that counts between each sample taken during a frame
  sampleTimer.setDurationMsec((double)(transRate));
  sampleTimer.start();
  sampleTimer.onUpdate([&]() {
        //this is what happens at the end of the timer
        sampleDone = true;
        bitsReceived++;
        if(bitsReceived == 10)
        {
          frameDone = true;
          Serial.println("frameDone");
          bitsReceived = 0;
        }
        //Serial.println("sample timer done");
        //Serial.println(micros());

    });
}

//interrupt handler
void edgeInterrupt(void)
{

  //detach the interrupt so that it doesnt trigger inside of a frame
  //detachInterrupt(digitalPinToInterrupt(interruptPin));
  //noInterrupts();
  //Serial.println("edge interrupt");
  //Serial.println(micros());
  
  receivedFrame = 0;
  frameDone = false;
  sampleDone = false; 
  volatile int sample = 0;
  
  while(frameDone == false)
  { 
    
    //Serial.println(frameDone);
    
    //trigger the sampling timer, if the timer is over then restart the timer, restart the timer for the next frame
    
    if(sampleTimer.isRunning() == 0)
    {
      sampleTimer.restart();
    }
    sampleTimer.update();
    
    while(sampleDone == false && frameDone == false) 
    {
      //Serial.println("sample bit time");
      if(digitalRead(DIN) == HIGH)
        sample++;
      else
        sample = sample -1;


    }    
    //reset the conditional
    sampleDone = false;
    //Serial.println(sample);

    if(sample > 0)
    {
      
      receivedFrame<<1;
      receivedFrame | 1;
      if(receivedFrame == 0)
      {
        receivedFrame = 1;
      }
    }
    else 
    {
      receivedFrame<<1;
    }
    //reset the sample counter
    sample = 0;
        
  }
  
  
  
  //Serial.println(receivedFrame);
  //frameHandler(receivedFrame);
  Serial.println("end of interrupt ");
  //digitalWrite(interruptControllerPin, HIGH);
  //Serial.println(micros());
  //reset the conditional
  //frameDone = false;
  //attachInterrupt(digitalPinToInterrupt(interruptPin), edgeInterrupt, RISING);
  //interrupts();  
}

void frameHandler(unsigned int inputFrame)
{
  //Serial.println("will handle frame");
  //check to see if the frame head and footer have been received
  //if(bitRead(inputFrame,9) == 1 && bitRead(inputFrame,0) == 0)
 // {
    //get rid of frame footer 
    //0111111110 is the mask, gets rid of all start/stop bits
    int bitMask = 255;
    dataVector.push_back(inputFrame & bitMask);
    //Serial.println("handled frame");
  //}
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
  /*if(frameDone == true)
    attachInterrupt(digitalPinToInterrupt(interruptPin), edgeInterrupt, RISING);*/
  

}