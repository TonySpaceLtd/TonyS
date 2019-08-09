#include <TonyS_X1.h>
#include "TonyS_X1_ExternalModule.h"

RS485  RS485(SLOT1);   //  Select slot such as SLOT1, SLOT1_U, SLOT2, SLOT2_U, SLOT3, SLOT3_U ...
                       //  SLOTx_U = Floor 2

void setup() 
{
  Serial.begin(115200);
  Serial.println("TonyS_X1 Example");
  Tony.begin();  //----  begin Library
  
  RS485.begin(115200);  // Serial from RS485 port   
  
  delay(10);
}

void loop() 
{
	
  //----------- Test print word to RS485 -----------//
  RS485.print("TEST RS485\r\n");
  delay(500); 
  
}