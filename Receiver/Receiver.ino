# include "Receiver.h"

# define CLK A1
# define DIN A0

Receiver receive(CLK, DIN);

void setup() {
  Serial.begin(9600);
}

void loop() {
  uint8_t readByte = receive.ReadByte();
  Serial.println((char)readByte);
}
