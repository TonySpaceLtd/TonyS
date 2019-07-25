#include <TonyS_X1.h>

TonyS_X1 Tony;   //  Call Library's class


void setup() 
{
  Serial.begin(115200);
  Serial.println("TonyS_X1 Example");
  Wire.begin();
  
  Tony.begin();  //----  begin Library
  delay(10);
  Tony.pinMode(IO13, OUTPUT); //----  Set Pin IO13 to OUTPUT (ON-OFF 3.3V on board)
}

void loop() 
{
  Tony.digitalWrite(IO13, HIGH); //---- Write HIGH to pin IO13 (ON 3.3V on board)
  delay(5000); 
  Tony.digitalWrite(IO13, LOW); //---- Write LOW to pin IO13 (OFF 3.3V on board)
  delay(5000); 
}