#include "Transmit.h"

#define CLK 10
#define DOUT 9

Transmit transmitter(CLK, DOUT);

void setup() {
  transmitter.setPeriod(10);
  transmitter.sendString("Hello World!");
}

void loop() {
  
}
