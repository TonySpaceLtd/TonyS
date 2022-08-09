#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

#define SLAVE_ID 0x03
#define ADDRESS  0
Tony_Modbus_Master modbus;
uint16_t dat[4]={1000,2000,3000,4000};
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
  modbus.presetMultipleRegisters(SLAVE_ID,ADDRESS,4,dat);
  for(uint8_t i=0;i<4;i++)
  {
    dat[i]+=10;
  }
  delay(1000);
}
