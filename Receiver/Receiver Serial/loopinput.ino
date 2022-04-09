// a 11111111 indicates the start and end of transmission

#include "Receiver.h"

# define CLK 9
# define DIN 7

# define maxSize 200

Receiver receive(CLK, DIN);


int state = 0;
int binaryInput = 7;


//byte array to store message
byte wholeBIN[maxSize]; //replace number with max number of characters

void setup() 
{
  //pinMode(binaryInput, INPUT);
  
  receive.setRGB(7, A3, A4);
  Serial.begin(9600);

}



String convertToString(byte bytes[maxSize])
{
  String convertedMessage=""; 
  //Serial.println(sizeof(bytes));
  for(int i=0; i<=maxSize;i++)
  {
    
     if(bytes[i] ==13)
      break;
     convertedMessage.concat((char)bytes[i]);
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
  for(int i=0; i<sizeof(wholeBIN); i++)
  {
    input = (byte)receive.ReadByte();
    //Serial.println(input);
    if(input==255)
    {
      //Serial.print("BREAK");
      break;
    }
    else
      wholeBIN[i] = input;
  }
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
      
      //Serial.print("\nthe data pin: ");
      //Serial.print(digitalRead(A0));
      //Serial.print("\nthe clk pin is: ");
      //Serial.print(digitalRead(A1));
      //Serial.println(receive.ReadByte());
      
      if(receive.ReadByte()==255)
      {
        getInputBinary();
        //Serial.println("transition");
        state=2;
      }
      else
        state=1;
      
      break;
    case 2:
      state = 3;
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
  

}
