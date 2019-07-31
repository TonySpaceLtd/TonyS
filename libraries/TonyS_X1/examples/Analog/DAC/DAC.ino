#include "TonyS_X1.h"

void setup() 
{
  Serial.begin(115200);
  Serial.println("TonyS_X1 Example");
  Tony.begin();  //----  begin Library
  delay(10);
}

void loop() 
{
  Tony.analogWrite(AIO3, 1024);   //---- Write analog pin AIO3 , 1024(2.5V) , Maximum = 4095(10V)  
  delay(3000);
  Tony.analogWrite(AIO3, 1311);   //---- Write analog pin AIO3 , 1311(3.2V) , Maximum = 4095(10V) 
  delay(3000);
}
