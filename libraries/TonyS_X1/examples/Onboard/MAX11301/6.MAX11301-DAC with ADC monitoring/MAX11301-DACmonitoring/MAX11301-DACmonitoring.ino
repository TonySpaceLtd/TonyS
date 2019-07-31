#include "TonyS_X1.h"

MAX11301 MAX;    //  Call Library's class

void setup() 
{
  
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  delay(10);

  Serial.println("Config Port...");
  //                               AIO0 = Port, 3.5 = DAC Output 3.5V
  MAX.Basic_Config_Port_For_DACADC(AIO0, 1434);  // DAC with monitoring to pin AIO0  , DAC Output = 1434(3.5V)   Maximum 4095(10V)
  Serial.println("Success !");  
  delay(100);
}

void loop() 
{
  uint16_t dataADC = 0;
  float Voltage = 0;
  
  //                    AIO0 = Pin
  dataADC = MAX.readADC(AIO0);  // Read ADC from pin AIO0 (DAC Output)

  Voltage = (10.0/4095)*dataADC;  // Calculate analog to voltage
  Serial.print("Voltage =   ");
  Serial.print(Voltage ,3);
  Serial.print("  V");
  Serial.println();
  Serial.println();
  delay(1000);
}
