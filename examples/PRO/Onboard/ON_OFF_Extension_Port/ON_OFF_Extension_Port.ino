#include "TonyS_X1.h"

void setup() 
{
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  delay(10);
  Serial.println("TonyS_X1 Example");
}

void loop() 
{
  onExtension(); //---- Call function on power extension port
  delay(5000); 
  offExtension(); //---- Call function off power extension port
  delay(5000); 
}
