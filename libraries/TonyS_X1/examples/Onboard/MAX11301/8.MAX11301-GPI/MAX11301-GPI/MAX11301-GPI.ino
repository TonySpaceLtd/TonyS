#include "TonyS_X1.h"

MAX11301 MAX;    //  Call Library's class

void setup() 
{
  
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  delay(10);

  Serial.println("Config Port...");
  //                            AIO0 = Pin, Threshold = 1638(1.0V)  MAX = 4095(2.5V)
  MAX.Basic_Config_Port_For_GPI(AIO0, 1638);  //   Set Pin AIO0 to GPI and Threshold = 1638(1.0V) , Maximum = 4095(2.5V)
  Serial.println("Success !");  
  delay(100);

  Serial.println("Config Port...");
  //                            AIO1 = Pin, Threshold = 2785(1.7V)  MAX = 4095(2.5V)
  MAX.Basic_Config_Port_For_GPI(AIO1, 2785); //   Set Pin AIO1 to GPI and Threshold = 2785(1.7V) , Maximum = 4095(2.5V)
  Serial.println("Success !");  
  delay(100);
}


//---------------------------------------- Note -------------------------------------------//

// This exaple use IO from MAX11301 , So you can't use Pin IO0, IO1, IO4, IO5, AIO00, AIO01.

//-----------------------------------------------------------------------------------------//


void loop() 
{
  bool dataGPI = 0;
  //                    AIO0 = Pin For read
  dataGPI = MAX.readGPI(AIO0);  //    Read GPI from port 0
  Serial.print("Data GPI AIO0: ");  
  Serial.println(dataGPI);
  Serial.println();
  delay(200);
  
  //                    AIO1 = Pin For read
  dataGPI = MAX.readGPI(AIO1);  //    Read GPI from port 19
  Serial.print("Data GPI AIO1: ");  
  Serial.println(dataGPI);
  Serial.println();
  delay(1000);
}
