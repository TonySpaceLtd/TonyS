#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"
void setup() {
  Serial.begin(115200);
  Serial.println("TonyS_X1 Example");
  Tony.begin();  //----  begin Library
  RS485.setSlot(SLOT4);
  RS485.begin(9600);
  
}

void loop() {
 if(RS485.available())
  Serial.write(RS485.read());
 if(Serial.available())
  RS485.write(Serial.read());
  

}
