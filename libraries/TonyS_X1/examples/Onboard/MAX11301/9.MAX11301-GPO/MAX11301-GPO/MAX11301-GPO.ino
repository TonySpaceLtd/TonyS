#include "TonyS_X1.h"

MAX11301 MAX;    //  Call Library's class

void setup() 
{
  
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  delay(10);

  Serial.println("Config Port...");
  //                            Pin, Output = 3562(8.7V)
  MAX.Basic_Config_Port_For_GPO(AIO0, 3563);  //   Set Pin AIO0 to GPO and Output = 3562(8.7V) , Maximum = 4095(10V)
  Serial.println("Success !");  
  delay(100);

  Serial.println("Config Port...");
  //                            Pin, Output = 1229(3V)
  MAX.Basic_Config_Port_For_GPO(AIO1, 1229);  //   Set Pin AIO1 to GPO and Output = 1229(3V) , Maximum = 4095(10V)
  Serial.println("Success !");  
  delay(100);
}

//---------------------------------------- Note -------------------------------------------//

// This exaple use IO from MAX11301 , So you can't use Pin IO0, IO1, IO4, IO5, AIO00, AIO01.

//-----------------------------------------------------------------------------------------//

void loop() 
{
  //---------- TEST High & Low Pin Pin AIO0 --------------//
  //           AIO0 = Pin, 1 = Output (High)
  MAX.writeGPO(AIO0, 1);   //---- Set 1 to High
  Serial.println("Pin AIO0 = High");
  Serial.println(); 
  delay(3000);

  //           AIO0 = Pin, 0 = Output (Low)
  MAX.writeGPO(AIO0, 0);
  Serial.println("Pin AIO0 = LOW");        //---- Set 0 to Low
  Serial.println();  
  delay(3000);
  //-----------------------------------------------//



  //---------- TEST High & Low Pin Pin AIO1 -----------//
  //           AIO1 = Pin, 1 = Output (High)
  MAX.writeGPO(AIO1, 1);    //---- Set 1 to High
  Serial.println("Pin AIO1 = High");
  Serial.println(); 
  delay(3000);

  //           AIO1 = Pin, 0 = Output (Low)
  MAX.writeGPO(AIO1, 0);   //---- Set 0 to Low
  Serial.println("Pin AIO1 = LOW");
  Serial.println();  
  delay(3000);
  //-----------------------------------------------//

}
