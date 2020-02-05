#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

void setup() 
{
    Serial.begin(9600);
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
  String Recv;
  while (RS485.available()) 
  {
    Recv = Recv+char(RS485.read());
  }
  if(Recv != "")
  {
    Serial.print("Get data : ");
    Serial.println(Recv);
  }
}