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
  uint16_t dataADC = 0;
  float Voltage = 0;
  dataADC = Tony.analogRead(AIO0);   //---- Read analog from pin AIO0
  Voltage = float(10.0/4095)*dataADC;  // Calculate analog to voltage
  Serial.print("Value = ");
  Serial.print(Voltage,3);  
  Serial.println(" V");  
  delay(1000); 
}