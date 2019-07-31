#include "TonyS_X1.h"

MAX11301 MAX;    //  Call Library's class

uint8_t pinADC[12] = {AIO0, AIO1, AIO2, AIO3, AIO4, AIO5, AIO6, AIO7, AIO8, AIO9, AIO10, AIO11};

void setup() 
{
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  delay(10);
  MAX.defaultConfig(); // Config port
  delay(10);
}

void loop() 
{

  //---------------- Test read ADC from port 0 - 11 -----------//
  for(uint8_t i=0; i<12; i++)
  {
    uint16_t dataADC = 0;
      //                         i = Pin for read
  	dataADC = MAX.readADC(pinADC[i]);  
    Serial.print("AIO");
    Serial.print(i);
    Serial.print("  ");
    Serial.print(dataADC);
    Serial.println();
    delay(1000);
  }
  delay(1000);
  Serial.println("\r\n\r\n");
  //-----------------------------------------------------------//
}
