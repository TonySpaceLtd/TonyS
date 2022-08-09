#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

#define SLAVE_ID 0x02
#define ADDRESS  0
Tony_Modbus_Master modbus;

void setup() 
{
  Serial.begin(115200);
  Serial.println("TonyS_X1 Example");
  Tony.begin();  //----  begin Library
  modbus.setSlot(SLOT4);
  modbus.begin(9600);
}
void loop() 
{
 if(modbus.readSingleInputStatus(SLAVE_ID,ADDRESS))
  Serial.println("ON");
 else
  Serial.println("OFF");
 delay(500);
}
