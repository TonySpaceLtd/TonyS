#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

T4_20mA  T4_20mA(SLOT1);   //  Select slot such as SLOT1, SLOT1_U, SLOT2, SLOT2_U, SLOT3, SLOT3_U ...
                           //   SLOTx_U = Floor 2

void setup() 
{
  Serial.begin(115200);
  Serial.println("TonyS_X1 Example");
  Tony.begin();  //----  begin Library
  delay(10);
}

void loop() 
{
  float milliamp = 0;
  uint16_t value = 0;
  
  value = T4_20mA.readValue();
  Serial.print("Value = ");
  Serial.println(value); 

  //----- Convert value to mA ------//
  milliamp = value/189.6;  //  1mA = 189.6   Calculate analog to voltage 
  Serial.print("Current = ");
  Serial.print(milliamp, 3);  
  Serial.println(" mA");  
  Serial.println();  
  delay(500); 
}