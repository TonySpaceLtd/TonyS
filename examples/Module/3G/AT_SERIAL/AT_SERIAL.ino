#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"



void setup() {
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  delay(10);
  gsm.initial(SLOT2);
 
}
void loop() {
  if(gsm.available() > 0) {
    Serial.write(gsm.read());
  }
  if(Serial.available() > 0) {
    gsm.write(Serial.read());
  }
}  
