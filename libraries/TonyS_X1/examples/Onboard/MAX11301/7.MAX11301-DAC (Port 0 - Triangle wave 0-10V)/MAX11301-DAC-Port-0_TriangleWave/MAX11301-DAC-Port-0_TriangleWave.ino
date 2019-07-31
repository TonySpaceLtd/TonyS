#include "TonyS_X1.h"

MAX11301 MAX;    //  Call Library's class

float count = 0;
float addPoint = 1;
uint16_t updown = 0;

void setup() 
{
  
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  delay(10);

  Serial.println("Config Port...");
  //                    AIO0 = Pin, DACtype = Rang DAC 0-10V);       
  MAX.Basic_Config_Port(AIO0, DACtype);                        // Config Port 0 to DAC 0-10V
  Serial.println("Success !");  
  delay(100);
  Serial.println();
  Serial.println("Please take oscilloscope for measure triangle wave...");  

}
  

void loop() 
{
  //------------ Triangle wave 0-10V -----------//
    if(updown == 0)
    {
    //             AIO0 = Pin , count = DAC Output
      MAX.writeDAC(AIO0, count);  
      count = count+addPoint;
      if(count >= 4095)
      {
        updown = 1;
      }
    }
    else
    {
      count = count-addPoint;
    //             AIO0 = Pin , count = DAC Output
      MAX.writeDAC(AIO0, count);
      if(count <= 1)
      {
        updown = 0;
      }
    }
    delayMicroseconds(100);
}
