#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"
  
void setup() 
{
  Serial.begin(115200);
  Tony.begin();
  
  TonyLORA.slot(SLOT2);   // Select the slot
  Serial.println();

  Serial.println("Testing send command to the module...");
  LoRaSerial.write("sip get_ver"); // Send command for asking firmware version of the chip
}

void loop() 
{
  /*
   If there are serial data come from the module
   It will print to you via Serial Monitor
   and you can send command directly to the module via Serial Monitor (No line ending)
   */
  if(LoRaSerial.available())
    Serial.write(LoRaSerial.read());
  if(Serial.available())
    LoRaSerial.write(Serial.read());
}