#include "Transmit.h"

#define CLK 10
#define DOUT 9

Transmit transmitter(CLK, DOUT);

void setup() {
  transmitter.setPeriod(2000);
  //transmitter.sendString("Hello World!");
  transmitter.setRGB(8, 7, 6);
  //transmitter.sendByte2('H');
  
}

void loop() {
  transmitter.sendString2("Hello World!\n");
}
