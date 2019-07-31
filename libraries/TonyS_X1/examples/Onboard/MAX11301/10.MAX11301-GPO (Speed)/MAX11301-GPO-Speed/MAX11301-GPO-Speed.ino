#include "TonyS_X1.h"

MAX11301 MAX;    //  Call Library's class

void setup() 
{
  
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  delay(10);

  Serial.println("Config Port...");
  //                            Port, Output = 3562(8.7V)
  MAX.Basic_Config_Port_For_GPO(AIO7, 3563);  //   Set AIO7 to GPO and Output = 3562(8.7V) , Maximum = 4095(10V)
  Serial.println("Success !");  
  delay(100);

  Serial.println("Config Port...");
  //                            Port, Output = 2867(7V)
  MAX.Basic_Config_Port_For_GPO(IO4, 2867);  //   Set IO4 to GPO and Output = 2867(7V) , Maximum = 4095(10V)
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
  //    ______________________________________________________________________________________________________________________________________
  //   |IC Port |  15  |  14  |  13  |  12  |   11  |   10  |    9  |    8  |    7  |    6  |     5  |     4  |    3  |    2  |    1  |    0  |
  //   |Pin IO  | IO9  | IO8  | IO7  | IO6  | AIO4  | AIO5  | AIO2  | AIO3  | AIO0  | AIO1  | AIO11  | AIO10  | AIO9  | AIO8  | AIO6  | AIO7  |
  //   |Bit     |  15  |  14  |  13  |  12  |   11  |   10  |    9  |    8  |    7  |    6  |     5  |     4  |    3  |    2  |    1  |    0  |  
  //   |BIN     |   0  |   0  |   0  |   0  |    0  |    0  |    0  |    0  |    0  |    0  |     0  |     0  |    0  |    0  |    0  |    1  |  = BIN 0000000000000001 
  //                                                                         														       ^     = HEX 0x01
  //                                                                            														   ^
  //                                                                            														   ^ AIO7 = 1  (High)
  //                 AIO7 = Port, 0x01 = Output
  MAX.write_speedGPO(AIO7, 0x01);
  Serial.println("AIO7 = High");
  Serial.println(); 
  delay(3000);

  //                 AIO7 = Port, 0x00 = Output
  MAX.write_speedGPO(AIO7, 0x00);  //---- 0x00 = Clear bit to 0 (Low)
  Serial.println("AIO7 = LOW");
  Serial.println();  
  delay(3000);
  //-----------------------------------------------//





  //---------- TEST High & Low Port 19 -----------//
  //   Port 16 - 19 = Full 4 Bit 
  //    _____________________________________
  //   |IC Port|   19  |   18  |  17  |  16  |
  //   |Pin IO | IO15  | IO14  | IO4  | IO5  |
  //   |Bit    |    3  |    2  |   1  |   0  |  
  //   |BIN    |    0  |    0  |   1  |   0  |  = BIN 0010 
  //                     		   ^            = HEX 0x02
  //                     		   ^
  //                     		   ^ IO4 = 1  (High)

  //                 IO4 = Port, 0x08 = Output
  MAX.write_speedGPO(IO4, 0x02);
  Serial.println("IO4 = High");
  Serial.println(); 
  delay(3000);

  //                 IO4 = Port, 0x00 = Output
  MAX.write_speedGPO(IO4, 0x00);   //---- 0x00 = Clear bit to 0 (Low)
  Serial.println("IO4 = LOW");
  Serial.println();  
  delay(3000);
  //-----------------------------------------------//
  
}
