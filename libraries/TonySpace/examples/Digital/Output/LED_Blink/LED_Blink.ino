#include <TonyS_X1.h>

TonyS_X1 Tony;   //  Call Library's class


void setup() 
{
  Serial.begin(115200);
  Serial.println("TonyS_X1 Example");
  Wire.begin();
  
  Tony.begin();  //----  begin Library
  delay(10);
  Tony.pinMode(IO12, OUTPUT); //----  Set Pin IO12 (LED_BUILTIN) to OUTPUT
}

void loop() 
{
  Tony.digitalWrite(IO12, HIGH); //---- Write HIGH to pin IO12 (LED_BUILTIN)
  delay(1000); 
  Tony.digitalWrite(IO12, LOW); //---- Write LOW to pin IO12 (LED_BUILTIN)
  delay(1000); 
}