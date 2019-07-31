#include "TonyS_X1.h"

void setup() 
{
  Serial.begin(115200);
  Serial.println("TonyS_X1 Example");
  Tony.begin();  //----  begin Library
  delay(10);
  Tony.pinMode(POWER_PIN, OUTPUT); //----  Set Pin IO13 to OUTPUT (ON-OFF 3.3V on board)
}

void loop() 
{
  Tony.digitalWrite(POWER_PIN, HIGH); //---- Write HIGH to pin IO13 (ON 3.3V on board)
  delay(5000); 
  Tony.digitalWrite(POWER_PIN, LOW); //---- Write LOW to pin IO13 (OFF 3.3V on board)
  delay(5000); 
}
