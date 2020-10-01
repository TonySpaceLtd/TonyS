#include "TonyS_X1.h"
HardwareSerial HWSerial(NULL);
void setup() {
  Serial.begin(9600);
  Serial.println("Hello World");
  Tony.begin();
  HWSerial = Tony.SerialBegin(SLOT2,9600);
}
void loop() {
 if(HWSerial.available())
    Serial.write(HWSerial.read());
if(Serial.available())
    HWSerial.write(Serial.read());
}