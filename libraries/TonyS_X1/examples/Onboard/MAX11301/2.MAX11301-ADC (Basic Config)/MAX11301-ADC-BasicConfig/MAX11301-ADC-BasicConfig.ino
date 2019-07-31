#include "TonyS_X1.h"

MAX11301 MAX;    //  Call Library's class

void setup() 
{
  
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  delay(10);

  Serial.println("Config Port...");
  //                    AIO0 = Pin, ADCtype1 = Rang for Read value   ---> ( ADCtype1 = 0-2.5V  , ADCtype1 = 0-10V )
  MAX.Basic_Config_Port(AIO0, ADCtype1);  // Config Pin AIO0 to ADC 0-10V
  Serial.println("Success !");  
  delay(100);
}

void loop() 
{
  uint16_t dataADC = 0;  
  
  dataADC = MAX.readADC(AIO0);  // Read ADC from pin AIO0
  
  Serial.print("AIO0 =   ");
  Serial.print(dataADC);
  Serial.println();
  Serial.println();
  delay(1000);
}
