#include "TonyS_X1.h"
HardwareSerial serialSlot1(NULL);
void setup() {
  Serial.begin(9600);
  Serial.println("Hello World");
  Tony.begin();
  serialSlot1 = Tony.SerialBegin(SLOT2,9600);
}
void loop() {
 if(serialSlot1.available())
    Serial.write(serialSlot1.read());
if(Serial.available())
    serialSlot1.write(Serial.read());
}