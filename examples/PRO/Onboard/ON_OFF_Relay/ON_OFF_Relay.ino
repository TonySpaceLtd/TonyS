#include "TonyS_X1.h"

void setup() 
{
  Serial.begin(115200);
  Serial.println("TonyS_X1 Example");
 
  Tony.begin();  //----  begin Library
  delay(10);
  Tony.pinMode(Relay_1, OUTPUT); //----  Set Pin IO14 (Relay 1) to OUTPUT
  Tony.pinMode(Relay_2, OUTPUT); //----  Set Pin IO15 (Relay 2) to OUTPUT
}
void loop() 
{
  Tony.digitalWrite(Relay_1, HIGH); //---- Write HIGH to pin IO14 (Relay 1)
  delay(1000); 
  Tony.digitalWrite(Relay_1, LOW); //---- Write LOW to pin IO14 (Relay 1)
  delay(1000); 
  
  Tony.digitalWrite(Relay_2, HIGH); //---- Write HIGH to pin IO15 (Relay 2)
  delay(1000); 
  Tony.digitalWrite(Relay_2, LOW); //---- Write LOW to pin IO15 (Relay 2)
  delay(1000); 
}
