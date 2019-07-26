#include <TonyS_X1.h>

TonyS_X1 Tony;   //  Call Library's class


void setup() 
{
  Serial.begin(115200);
  Serial.println("TonyS_X1 Example");
  Wire.begin();
  
  Tony.begin();  //----  begin Library
  delay(10);
}

void loop() 
{
  Tony.analogWrite(AIO0, 1024);   //---- Write analog pin AIO0 , 1024(2.5V) , Maximum = 4095(10V)  
  delay(3000);
  Tony.analogWrite(AIO0, 1311);   //---- Write analog pin AIO0 , 1311(3.2V) , Maximum = 4095(10V) 
  delay(3000);
}