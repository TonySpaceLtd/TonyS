#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

#define SLAVE_ID 0x02
#define ADDRESS  0
#define NUMDATA  10
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
 uint8_t *buf;
 uint16_t len;
 if(modbus.readMultiInputStatus(SLAVE_ID,ADDRESS,NUMDATA,&buf,&len))
 {
    for(uint16_t i=0;i<len;i++)
    {
      Serial.print("Buffer ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(buf[i],BIN);
    }
 }
 delay(1000);
}
