#include <TonyS_X1.h>

TonyS_X1 Tony;   //  Call Library's class
MAX11301 MAX;    //  Call Library's class


void setup() 
{
  
  Serial.begin(115200);
  Wire.begin();
  Tony.begin();  //----  begin Library
  delay(10);

  Serial.println("Config Port...");
  //                            Port, Output = 3562(8.7V)
  MAX.Basic_Config_Port_For_GPO(0, 3563);  //   Set Port 0 to GPO and Output = 3562(8.7V) , Maximum = 4095(10V)
  Serial.println("Success !");  
  delay(100);

  Serial.println("Config Port...");
  //                            Port, Output = 2867(7V)
  MAX.Basic_Config_Port_For_GPO(17, 2867);  //   Set Port 17 to GPO and Output = 2867(7V) , Maximum = 4095(10V)
  Serial.println("Success !");  
  delay(100);
}

//---------------------------------------- Note -------------------------------------------//

// This exaple use IO from MAX11301 , Please open schemetic for see directly ports of chip(MAX11301)

//-----------------------------------------------------------------------------------------//

void loop() 
{
  //---------- TEST High & Low Port 0 --------------//
  //   Port 0 - 15 = Full 16 Bit 
  //    __________________________________________________________________________
  //   |Port| 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
  //   |Bit | 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |  
  //   |BIN |  0 | 0  | 0  | 0  | 0  | 0  | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 |  = BIN 0000000000000001 
  //                                                                            ^    = HEX 0x01
  //                                                                            ^
  //                                                                            ^ Port 0 = 1  (High)
  //                 0 = Port, 0x01 = Output
  MAX.write_speedGPO(0, 0x01);
  Serial.println("Port 0 = High");
  Serial.println(); 
  delay(3000);

  //                 0 = Port, 0x00 = Output
  MAX.write_speedGPO(0, 0x00);  //---- 0x00 = Clear bit to 0 (Low)
  Serial.println("Port 0 = LOW");
  Serial.println();  
  delay(3000);
  //-----------------------------------------------//





  //---------- TEST High & Low Port 19 -----------//
  //   Port 16 - 19 = Full 4 Bit 
  //    __________________________________________________________________________
  //   |Port| 19 | 18 | 17 | 16 |
  //   |Bit |  3 |  2 |  1 |  0 |  
  //   |BIN |  0 |  0 |  1 |  0 |  = BIN 0010 
  //                     ^         = HEX 0x02
  //                     ^
  //                     ^ Port 17 = 1  (High)

  //                 17 = Port, 0x08 = Output
  MAX.write_speedGPO(17, 0x02);
  Serial.println("Port 17 = High");
  Serial.println(); 
  delay(3000);

  //                 17 = Port, 0x00 = Output
  MAX.write_speedGPO(17, 0x00);   //---- 0x00 = Clear bit to 0 (Low)
  Serial.println("Port 17 = LOW");
  Serial.println();  
  delay(3000);
  //-----------------------------------------------//
  
}
