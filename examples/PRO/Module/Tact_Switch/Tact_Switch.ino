#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"


void setup()
{
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  TonyTactSW.begin(SLOT1);  //---- Select Slot of module


  // ------- TEST BUZZER ----------//
  TonyTactSW.onBuzzer();
  delay(500);
  TonyTactSW.offBuzzer();

  //---------- TEST LED -----------//
  for(uint8_t i=0; i<5; i++)
  {
    TonyTactSW.onLED1();  // ON LED 1
    delay(500);
    TonyTactSW.offLED1();  // OFF LED 1
    TonyTactSW.onLED2();  // ON LED 2
    delay(500);
    TonyTactSW.offLED2();  // OFF LED 2
  }
}

void loop()
{
  // Read port by port
  for(uint8_t i=0; i<5; i++)
  {
    uint16_t dataADC = TonyTactSW.read(i);
    Serial.print("Switch ");
    Serial.print(i);  
    Serial.print(" = ");
    Serial.println(dataADC);  
  }

  // Read all port (This method is faster than read port by port
  uint8_t data_all = TonyTactSW.readAll();
  Serial.print("DATA ALL PORTS ");
  Serial.print(" = 0x000");
  if(data_all < 0b00010000)Serial.print("0");
  Serial.println(data_all ,BIN);  
  Serial.println();  
  delay(100);
}