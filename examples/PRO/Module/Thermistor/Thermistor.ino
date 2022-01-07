#include "TonyS_X1.h"
/*
             Selection Pin
 ______________________________________
| SLOT | Pin 1st floor | Pin 2st floor |
 --------------------------------------
|  1   |      AIO0     |      AIO1     |
|  2   |      AIO2     |      AIO3     |
|  3   |      AIO4     |      AIO5     |
|  4   |      AIO6     |      AIO7     |
|  5   |      AIO8     |      AIO9     |
|  6   |      AIO10    |      AIO11    |
 --------------------------------------


           Reference value @ 32.50°C
 _____________________________________________
|        Input         | High Temp | Low Temp |
 ---------------------------------------------
| NTC(Onboard)         |    770    |   1075   |
| External Thermistor  |           |          |
----------------------------------------------
         
 */


void setup() 
{
  Serial.begin(115200);
  Serial.println("TonyS_X1 Example");
  Tony.begin();  //----  begin Library
  Serial.println();
  Serial.println("Testing Thermistor Module");
  Serial.println();
  delay(10);
}

void loop() 
{
/*
Example  reading temperature by NTC(Onboard) and select jumper at LOW Temp
So, 32.50°C = 1075(Digital value)
32.5/1075 = 0.0302325581395349 (Calibrate value)
*/
  
  uint16_t readdata = 0;
  float temp = 0;
  readdata = Tony.analogRead(AIO0);     // Reading from slot 1 on 1st floor = AIO0
  temp = 0.0302325581395349*readdata;  // Calculate analog to voltage
  Serial.print("Digital Value = ");
  Serial.println(readdata);   
  Serial.print("Temperature = ");
  Serial.print(temp);  
  Serial.println(" °C");  
  Serial.println();   
  delay(1000); 
}