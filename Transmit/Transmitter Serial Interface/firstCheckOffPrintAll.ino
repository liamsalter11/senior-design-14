//vars for getting string
String userString;

byte charByte[7]; //array for converting char into bin
byte charByteSum=0; //array for store the final value of a char's bin

byte wholeBIN[100]; //replace the number with the max number of characters

//loop vars 
int state=0;

char line[80];
int count = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Starting\n");
}

void convertString2Binary(String inputMessage)
{
 int messageLength = inputMessage.length();
 char inputChars [messageLength]; 
 char inByte;
 int inputPosition=0;
 
 char letter;
 int bytes[messageLength];
 
  
 for(int i=0; i<messageLength; i++)
 {

   letter = inputMessage.charAt(i);
   wholeBIN[i] = inputMessage.charAt(i);
    for(int j=7; j>=0; j--)
    {
      //get the bit value of the the first byte in a 
      charByte[j] = bitRead(letter,j);
      Serial.print(charByte[j], BIN);
      
      
     
    }

    //add it to the whole binary message
        
    Serial.print("  which is ");
    Serial.print(letter);
    
    Serial.println("");
    
 }
}

void startUpPrompt(void)
{
  delay(3000);
  Serial.print("Hello! You can input a message below and have it turned into binary!\n");
  Serial.print("Enter Message: ");
  
   
}

bool getInputMessage(void)
{
    if (Serial.available() > 0) 
    {

      line[count] = (char)Serial.read(); // store the char
      if (line[count++] == '\r'){        // if its a CR,
         line[count] = '\0';             //  zero-terminate it
         Serial.print("----------\nYour message is: ");
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

void printAllBinary(void)
{ 
  Serial.println("----------");
  Serial.print("Whole message is:\n");
  for(int i=0; i<sizeof(wholeBIN);i++)
  {
    //Serial.print("print loop");
    Serial.print(wholeBIN[i],BIN);
    Serial.print(" ");
  }
  Serial.println("\n----------");
}

void resetSystem(void)
{
  userString="";
  line[0]='\0';
  count=0;
  Serial.flush();
  Serial.print("\nRestarting..... \n \n");
}

void loop() 
{
  // put your main code here, to run repeatedly:
 
  
  
  switch(state)
  {
    case 0: //startup state
      startUpPrompt();
      delay(10);
      state=1;
      break;
  
    case 1: //conver the message to binary
      if(getInputMessage() == true)
      {
        convertString2Binary(userString);
        printAllBinary();
        state=2;
      }
      else
      {
        delay(10);
        state=1;
      }
      break;
   
    case 2:
      delay(1000);
      resetSystem();
      state=0;
      break;
     
  }


  
  delay(200);
  
}
