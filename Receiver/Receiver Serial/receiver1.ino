// a 11111111 indicates the start and end of transmission

#include "Receiver.h"

# define CLK A1
# define DIN A0

Receiver receive(CLK, DIN);


int state = 0;
int binaryInput = 7;


//byte array to store message
byte wholeBIN[100]; //replace number with max number of characters

void setup() 
{
  //pinMode(binaryInput, INPUT);
  receive.setRGB(A2, A3, A4);
  Serial.begin(9600);

}



String convertToString(byte bytes[100])
{
  String convertedMessage=""; 
  for(int i=0; i<sizeof(bytes);i++)
  {
     if(bytes[i] !=0)
      convertedMessage.concat(bytes[i]);
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
  Serial.println("----------\n");
}



void getInputBinary(void)
{
  byte input;
  for(int i=0; i<sizeof(wholeBIN); i++)
  {
    input = (byte)receive.ReadByte2();
    if(input=255)
      break;
    else
      wholeBIN[i] = input;
  }
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
  
  switch(state)
  {
    case 0:
      printStartUpMessage();
      state=1;
      break;
    
    case 1:
      if(receive.ReadByte2()==255)
        state=2;
      else
        state=1;
      break;
    case 2:
      getInputBinary();
      state = 3;
      break;
    case 3:
      printMessageToSerial(convertToString(wholeBIN));
      state = 4;
    case 4:
      resetSystem();
      state=1;
      break; 
      
  }
  

}
