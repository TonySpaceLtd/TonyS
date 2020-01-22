#include "TonyS_X1.h"

void setup() 
{
  Serial.begin(115200);
  Serial.println("TonyS_X1 Example");
  
  Tony.begin();  //----  begin Library
  delay(10);
  Tony.pinMode(LED_BUILTIN, OUTPUT); //----  Set Pin IO12 (LED_BUILTIN) to OUTPUT
}

void loop() 
{
  Tony.digitalWrite(LED_BUILTIN, HIGH); //---- Write HIGH to pin IO12 (LED_BUILTIN)
  delay(1000); 
  Tony.digitalWrite(LED_BUILTIN, LOW); //---- Write LOW to pin IO12 (LED_BUILTIN)
  delay(1000); 
}
