#include "TonyS_X1.h"

void setup() 
{
  Serial.begin(115200);
  Serial.println("TonyS_X1 Example");
  Tony.begin();  //----  begin Library
  delay(10);
}

void loop() 
{
  uint16_t dataADC = 0;
  float Voltage = 0;
  dataADC = Tony.analogRead(AIO0);     // Read analog from pin AIO0
  Voltage = float(10.0/4095)*dataADC;  // Calculate analog to voltage
  Serial.print("AIO0 Value = ");
  Serial.print(Voltage,3);  
  Serial.println(" V");  
  Serial.print("AIO0 Digital Value = ");
  Serial.println(dataADC);  


  /*
   Changing ADC voltage range 
   ADC10V = ADC 0-10V at 12 Bit (Default)
   ADC2V5 = ADC 0-2.5V at 12 Bit
  (If the function (Tony.pinMode()) is not called, Ports will be 10V automatically)
   Example 
   Tony.pinMode(AIO0, ADC10V); // Set ADC voltage range 0 to 10V at AIO0 Pin
   Tony.pinMode(AIO0, ADC2V5); // Set ADC voltage range 0 to 2.5V at AIO0 Pin
  */
  dataADC = 0;
  Voltage = 0;
  Tony.pinMode(AIO2, ADC2V5);          // Set ADC voltage range 0 to 2.5V at AIO2 Pin
  dataADC = Tony.analogRead(AIO2);     // Read analog from pin AIO2
  Voltage = float(2.5/4095)*dataADC;   // Calculate analog to voltage
  Serial.print("AIO2 Value = ");
  Serial.print(Voltage,3);  
  Serial.println(" V");  
  Serial.print("AIO2 Digital Value = ");
  Serial.println(dataADC);  
  Serial.println();
  delay(1000); 
}
