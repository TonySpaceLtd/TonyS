#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

void setup() 
{
    Serial.begin(115200);
    Serial.println("TonyS_X1 Example");
    Tony.begin();  //----  begin Library
    
//--------- RS485 Module Config ---------//
    RS485.slot(SLOT1);    //  Select slot such as SLOT1, SLOT1_U, SLOT2, SLOT2_U, SLOT3, SLOT3_U ...
                          //  SLOTx_U = Floor 2
    RS485.begin(9600);  // Select buad rate 
//--------------------------------------//
}

void loop() 
{
  //--------------------- ON -------------------//
   //RS485.write_singleCoil(Slave ID, Address, Value);   Value 0=OFF , 1=ON
  if(RS485.write_singleCoil(1, 3, 1)) Serial.println("The command has been written : ON");  //Return 1 = Sent success , and Return 0 = Not success
  else Serial.println("Failed.");
  delay(500);

  //--------------------- OFF ------------------//
  if(RS485.write_singleCoil(1, 3, 0)) Serial.println("The command has been written : OFF");  //Return 1 = Sent success , and Return 0 = Not success
  else Serial.println("Failed.");
  delay(500);
}