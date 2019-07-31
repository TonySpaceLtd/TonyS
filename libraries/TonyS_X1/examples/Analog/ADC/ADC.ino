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
  uint16_t dataADC = 0;
  float Voltage = 0;
  dataADC = Tony.analogRead(AIO2);   //---- Read analog from pin AIO2
  Voltage = float(10.0/4095)*dataADC;  // Calculate analog to voltage
  Serial.print("Value = ");
  Serial.print(Voltage,3);  
  Serial.println(" V");  
  delay(1000); 
}
