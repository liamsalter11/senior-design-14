int state = 0;
int binaryInput = 7;


//byte array to store message
byte wholeBIN[100]; //replace number with max number of characters

void setup() 
{
  pinMode(binaryInput, INPUT);

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

void getInputBinary(void)
{
  int flag = 0;

  while(flag == 0)
  {
    
  }
}

void loop() 
{
  
  switch(state)
  {
    case 0:
      break;
    case 1:
      break;
  }
  

}
