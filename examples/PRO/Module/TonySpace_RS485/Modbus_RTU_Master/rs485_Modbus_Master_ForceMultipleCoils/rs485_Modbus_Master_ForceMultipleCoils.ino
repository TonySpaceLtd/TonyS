#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

#define SLAVE_ID 0x01
#define ADDRESS  0

Tony_Modbus_Master modbus;
uint8_t dat[2];
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
 dat[0]=0xFF;
 dat[1]=0x01;
 modbus.forceMultipleCoils(SLAVE_ID,ADDRESS,10,dat);
 delay(500);
 
 dat[0]=0x00;
 dat[1]=0x02;
 modbus.forceMultipleCoils(SLAVE_ID,ADDRESS,10,dat);
 delay(500);
}
