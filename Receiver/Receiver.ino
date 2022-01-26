# include "Receiver.h"

# define CLK A1
# define DIN A0

Receiver receive(CLK, DIN);

void setup() {
  Serial.begin(9600);
  receive.setRGB(A2, A3, A4);
}

void loop() {
  uint8_t readByte = receive.ReadByte2();
  Serial.print((char)readByte);
  //Serial.print(analogRead(A2));
  //Serial.print(' ');
  //Serial.print(analogRead(A3));
  //Serial.print(' ');
  //Serial.println(analogRead(A4));
  
}
