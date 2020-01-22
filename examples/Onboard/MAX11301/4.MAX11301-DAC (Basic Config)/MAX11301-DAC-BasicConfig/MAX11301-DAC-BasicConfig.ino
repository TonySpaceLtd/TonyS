#include "TonyS_X1.h"

MAX11301 MAX;    //  Call Library's class

void setup() 
{
  
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  delay(10);

  Serial.println("Config Port...");
  //                    AIO0 = Port, DACtype = Rang DAC 0-10V);       // Read about config above
  MAX.Basic_Config_Port(AIO0, DACtype);                        // Config Port 0 to DAC 0-10V
  Serial.println("Success !");  
  delay(100);


  //---------------- Write DAC ----------------//
  //           Pin, Output DAC 0-10V
  MAX.writeDAC(AIO0, 1434);  // Write DAC to pin AIO0 1434(3.5V)   Maximum 4095(10V)

  Serial.println("Please measure voltage at Port 0");  
}

void loop() 
{
}
