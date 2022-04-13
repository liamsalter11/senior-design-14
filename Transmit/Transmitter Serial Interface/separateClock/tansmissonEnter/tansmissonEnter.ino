#include <Vector.h>


//constant for defining the max size
# define maxSize 200

# define baud 1200

#define maxFrames 100

//vars for getting string
String userString;

byte charByte[7]; //array for converting char into bin
byte charByteSum=0; //array for store the final value of a char's bin

byte wholeBIN[maxSize]; //replace the number with the max number of characters

unsigned int storageArray[maxFrames];
Vector<unsigned int> frames(storageArray);


//loop vars 
int state;

char line[100];
int count = 0;

//blinking variables
//green =12
//blue = 11
//red = 13
char ledMessage[7];
const int binaryLED = 11; //blue
const int binaryLEDg = 12;
const int binaryLEDr = 13;
const int clockLED = 4;


const int transRate = 1;  //100ms
const int frameDelay = 5; //300ms
//10 bits in a frame, 0-9
const int frameSize = 9;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  Serial.print("Starting\n");

  
  pinMode(binaryLED, OUTPUT);
  pinMode(binaryLEDg, OUTPUT);
  pinMode(binaryLEDr, OUTPUT);
  
  pinMode(clockLED, OUTPUT);

  digitalWrite(clockLED, LOW);
  digitalWrite(binaryLED, LOW);
  digitalWrite(binaryLEDg, LOW);
  digitalWrite(binaryLEDr, LOW);

  state = 1;
}

void convertString2Binary(String inputMessage)
{
  //add header
  addTransmissionStartEnd();
  int messageLength = inputMessage.length();
  char inputChars[messageLength]; 
  char inByte;
  int inputPosition=0;
  
  char letter;
  int bytes[messageLength];
  
    
  for(int i=0; i<messageLength-1; i++)
  {
    letter = inputMessage.charAt(i);
    wholeBIN[i] = inputMessage.charAt(i);
    frames.push_back(generateFrame(letter));  
  }
  //add footer
  addTransmissionStartEnd();
}

void startUpPrompt(void)
{
  //delay(3000);
  Serial.println("Hello! You can input a message below and have it turned into binary!");
  //double hertz = 1/(transRate);
  //Serial.print(hertz);
 // Serial.print(" khz");
  Serial.print("Enter Message: ");
  
   
}

bool getInputMessage(void)
{
  if (Serial.available() > 0) 
  {
  
    line[count] = (char)Serial.read(); // store the char
    if (line[count++] == '\n'){        // if its a CR,
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
  for(unsigned int i = 0; i < frames.size(); i++)
  {
    //Serial.print("print loop");
    /*if(wholeBIN[i] == 0)
      continue;*/
    Serial.print(frames.at(i),BIN);
    
    Serial.print(" ");
  }
  Serial.println("\n----------");
}

void resetSystem(void)
{
  userString="";
  line[0]='\0';
  count=0;
  for(unsigned int i = 0; i < maxSize; i++)
  {
    wholeBIN[i] = 0;
  }

  frames.clear();
  digitalWrite(clockLED, LOW);
  digitalWrite(binaryLED, LOW);
  digitalWrite(binaryLEDg, LOW);
  digitalWrite(binaryLEDr, LOW);
  Serial.flush();
  Serial.print("\nRestarting..... \n \n");
}


void addTransmissionStartEnd(void)
{
    frames.push_back(generateFrame(255));
    Serial.println("adding header/footer");
}

unsigned int generateFrame(byte charByte)
{
  //frame is: [header][zero pad][data][footer]

  //header is binary 1
  unsigned int frame = 1;

  //shift in and add byte
  frame = frame << 8;
  frame |= charByte;

  //add footer which is zero
  frame = frame << 1;
  //frame = frame | 1;
  
 /* Serial.print("\n------ frame:");
  Serial.print(frame); 
  Serial.print("  ");
  Serial.print(frame,BIN); 
  Serial.print("-----\n");*/

  return frame;
}


void printFrames(void)
{
  
  for(unsigned int i =0; i<frames.size();i++)
  {
    for(unsigned int j = 0; j<frameSize; j++)
    {
      //Serial.print("frameSize");
      uint8_t singleBit = bitRead(frames.at(i),frameSize-j);
      digitalWrite(binaryLED, singleBit);

      //wait a clock period before sending another bit
      unsigned int waitTime = transRate * 1000 + micros();
      while(micros() <= waitTime);
    }
    
    digitalWrite(binaryLED, LOW);
    
    //delay between frames
    unsigned int frameWaitTime = frameDelay * 1000 + micros();
    while(micros() <= frameWaitTime);
  }
}

void printFramesRGB(void)
{
  // PortA 16, 19, 17
  // Pins  11, 12, 13
  for(unsigned int i = 0; i < frames.size(); i++)
  {
    for(unsigned int j = 0; j < frameSize; j += 3)
    {
      //Serial.print("frameSize");
      uint8_t bitR = bitRead(frames.at(i), frameSize - j + 2);
      uint8_t bitG = bitRead(frames.at(i), frameSize - j + 1);
      uint8_t bitB = bitRead(frames.at(i), frameSize - j);
      digitalWrite(binaryLED, bitB);
      digitalWrite(binaryLEDg, bitG);
      digitalWrite(binaryLEDr, bitR);

      //wait a clock period before sending another bit
      unsigned int waitTime = transRate * 1000 + micros();
      while(micros() <= waitTime);
       
    }
    digitalWrite(binaryLED, LOW);
    digitalWrite(binaryLEDg, LOW);
    digitalWrite(binaryLEDr, LOW);
    //delay between frames
    unsigned int frameWaitTime = frameDelay * 1000 + micros();
    while(micros() <= frameWaitTime); 
  }
}


void loop() 
{
  switch(state)
  {
    case 0: //startup state
      //startUpPrompt();
      //delay(10);
      state=1;
  
    case 1: //conver the message to binary
      if(getInputMessage())
      {
        convertString2Binary(userString);
        
        printFramesRGB();
        printAllBinary();
        state=2;
      }
      else
      {
        //delay(10);
        state=1;
        break;
      }

    case 2:
      //delay(1000);
      resetSystem();
      state=0;
      break;
      
    default:
      state = 2;
  }

  //delay(200);
}
