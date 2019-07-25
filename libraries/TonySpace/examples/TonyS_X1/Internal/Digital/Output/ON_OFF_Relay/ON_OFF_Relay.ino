#include <TonyS_X1.h>

TonyS_X1 Tony;   //  Call Library's class


void setup() 
{
  Serial.begin(115200);
  Serial.println("TonyS_X1 Example");
  Wire.begin();
  
  Tony.begin();  //----  begin Library
  delay(10);
  Tony.pinMode(IO14, OUTPUT); //----  Set Pin IO14 (Relay 1) to OUTPUT
  Tony.pinMode(IO15, OUTPUT); //----  Set Pin IO15 (Relay 2) to OUTPUT
}

void loop() 
{
  Tony.digitalWrite(IO14, HIGH); //---- Write HIGH to pin IO14 (Relay 1)
  delay(1000); 
  Tony.digitalWrite(IO14, LOW); //---- Write LOW to pin IO14 (Relay 1)
  delay(1000); 
  
  Tony.digitalWrite(IO15, HIGH); //---- Write HIGH to pin IO15 (Relay 2)
  delay(1000); 
  Tony.digitalWrite(IO15, LOW); //---- Write LOW to pin IO15 (Relay 2)
  delay(1000); 
}