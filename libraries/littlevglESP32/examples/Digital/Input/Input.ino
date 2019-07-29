#include <TonyS_X1.h>

TonyS_X1 Tony;   //  Call Library's class


void setup() 
{
  Serial.begin(115200);
  Serial.println("TonyS_X1 Example");
  Wire.begin();
  
  Tony.begin();  //----  begin Library
  delay(10);
  Tony.pinMode(IO1, INPUT); //----  Set Pin IO1 to INPUT
}

void loop() 
{
  bool value = 0;
  value = Tony.digitalRead(IO1); //---- Read pin IO1 
  Serial.print("Value : ");
  Serial.println(value);
  delay(1000); 
}