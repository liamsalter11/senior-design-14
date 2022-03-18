//constant for defining the max size
# define maxSize 200

//vars for getting string
String userString;

byte charByte[7]; //array for converting char into bin
byte charByteSum=0; //array for store the final value of a char's bin

byte wholeBIN[maxSize]; //replace the number with the max number of characters

//loop vars 
int state=0;

char line[80];
int count = 0;

//blinking variables
//green =12
//blue = 11
//red = 13
char ledMessage[7];
int binaryLED = 11; //green
int binaryLEDb = 11;
int binaryLEDr = 13;
int clockLED = 4;
int transRate = 20;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Starting\n");

  
  pinMode(binaryLED, OUTPUT);
  pinMode(binaryLEDb, OUTPUT);
  pinMode(binaryLEDr, OUTPUT);
  
  pinMode(clockLED, OUTPUT);

  digitalWrite(clockLED, LOW);
  digitalWrite(binaryLED, LOW);
  digitalWrite(binaryLEDb, LOW);
  digitalWrite(binaryLEDr, LOW);
  
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
      //Serial.print(charByte[j], BIN);
      printLED(bitRead(letter,j));
      
     
    }

    //add it to the whole binary message
      
    //Serial.print("  which is ");
    //Serial.print(letter);
    
    //Serial.println("");
    
 }
}

void startUpPrompt(void)
{
  delay(3000);
  Serial.print("Hello! You can input a message below and have it turned into binary!");
  //double hertz = 1/(transRate);
  //Serial.print(hertz);
 // Serial.print(" khz");
  Serial.println();
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
  for(int i=0; i<maxSize;i++)
  {
    //Serial.print("print loop");
    if(wholeBIN[i] == 0)
      continue;
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
  for(int i = 0; i<maxSize;i++)
  {
    wholeBIN[i] = 0;
    
  }
  digitalWrite(clockLED, LOW);
  digitalWrite(binaryLED, LOW);
  digitalWrite(binaryLEDb, LOW);
  digitalWrite(binaryLEDr, LOW);
  Serial.flush();
  Serial.print("\nRestarting..... \n \n");
}

void printLED(int input)
{
  
  if(input == 1)
    {
      digitalWrite(binaryLED, HIGH);
      //delay(transRate/2);
    }
   else
   {
      digitalWrite(binaryLED, LOW);
      //delay(transRate/2);
   }
   digitalWrite(clockLED,HIGH);
   delay(transRate/2);
   digitalWrite(clockLED,LOW);
   delay(transRate/2);
  
  
}

void sendTransmissionStartEnd(void)
{
    //digitalWrite(clockLED,!digitalRead(clockLED));
    digitalWrite(binaryLED, HIGH);
    
    for(int i=0; i<8; i++)
    {
      digitalWrite(clockLED,HIGH);
      
      delay(transRate/2);
      digitalWrite(clockLED,LOW);
      delay(transRate/2);
     // digitalWrite(binaryLED, !digitalRead(binaryLED));
    }
    
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
        sendTransmissionStartEnd();
        convertString2Binary(userString);
        sendTransmissionStartEnd();
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


  
  //delay(200);
  
}
