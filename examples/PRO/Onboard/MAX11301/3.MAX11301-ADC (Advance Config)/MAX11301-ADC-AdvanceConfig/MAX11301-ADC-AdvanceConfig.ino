#include "TonyS_X1.h"

MAX11301 MAX;    //  Call Library's class



//------------------------------------ Advance Config Port (Start) ------------------------//    

//#define Mode      0x07  // Mode selection   // 0x00 : Mode 0 =  High impedance
                          // 0x01 : Mode 1 =  Digital input with programmable threshold (GPI)
                          // 0x02 : Mode 2 =  Bidirectional level translator terminal
                          // 0x03 : Mode 3 =  Register-driven digital output with DAC-controlled level (GPO)
                          // 0x04 : Mode 4 =  Unidirectional path output with DAC-controlled level (GPO)
                          // 0x05 : Mode 5 =  Analog output for DAC
                          // 0x06 : Mode 6 =  Analog output for DAC with ADC monitoring
                          // 0x07 : Mode 7 =  Positive analog input to single-ended ADC
                          // 0x08 : Mode 8 =  Positive analog input to differential ADC
                          // 0x09 : Mode 9 =  Negative analog input to differential ADC
                          // 0x0a : Mode 10 = Analog output for DAC and negative analog input to differential ADC
                          // 0x0b : Mode 11 = Terminal to GPI-controlled analog switch
                          // 0x0c : Mode 12 = Terminal to register-controlled analog switch  
                        
//#define AVR_INV      0x00    // If use ADC Mode    0x00 : ADC internal voltage reference         
                               //                    0x01 : ADC external voltage reference
                     // If use GPI-controlled Mode    0x00 : Data received from GPI-configured port is not inverted
                     //                               0x01 : Data received from GPI-configured port is inverted                                                       
 
//#define RANGE    0x01   //Determines the input voltage range of ports configured in input modes, or the output voltage range of ports configured in output modes.     
//                         _________________________________________________________________________
//                        | VOLTAGE RANGE CODES  |  ADC VOLTAGE RANGE (V)  |  DAC VOLTAGE RANGE (V) |
//                         -------------------------------------------------------------------------
//                        | 0x00                 | No Range Selected       | No Range Selected      |
//                        | 0x01                 | 0 to +10                | 0 to +10               |
//                        | 0x02                 | -5 to +5                | -5 to +5               |
//                        | 0x03                 | -10 to 0                | -10 to 0               |
//                        | 0x04                 | 0 to +2.5               | -5 to +5               |
//                        | 0x05                 | Reserved                | Reserved               |
//                        | 0x06                 | 0 to +2.5               | 0 to +10               |
//                        | 0x07                 | Reserved                | Reserved               |
//                         -------------------------------------------------------------------------   
 
 
//#define SAMPLES    0x07   //Defines the number of samples   // 0x00 : Samples = 1
                                // 0x01 : Samples = 2                        
                                // 0x02 : Samples = 4    
                                // 0x03 : Samples = 8
                                // 0x04 : Samples = 16
                                // 0x05 : Samples = 32
                                // 0x06 : Samples = 64
                                // 0x07 : Samples = 128 
                               
//#define ASSOCIATED    0x00    // ASSOCIATED PORT  (Defines the port to use in conjunction with a port configured in mode 4, 8, or 11.)
                      //(Set to 0x00 If not use mode 4 , 8 or 11)
                      // 0x04 : Mode 4 =  Unidirectional path output with DAC-controlled level (GPO)
                      // 0x08 : Mode 8 =  Positive analog input to differential ADC
                      // 0x0b : Mode 11 = Terminal to GPI-controlled analog switch  
                      
//------------------------------------ Config Port (End) ------------------------//     




void setup() 
{
  
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  delay(10);

  Serial.println("Config Port...");
  //                      AIO0 = Port , Mode, AVR_INV, RANGE, SAMPLES, ASSOCIATED);       // Read about config above
  MAX.Advance_Config_Port(AIO0, 0x07, 0x00,    0x01,  0x07,    0x00);  // Config Port 0 to ADC 0-10V
  Serial.println("Success !");  
  delay(100);
}

void loop() 
{
  uint16_t dataADC = 0;  
  
  dataADC = MAX.readADC(AIO0);  // Read ADC from pin AIO0
  
  Serial.print("AIO0 =   ");
  Serial.print(dataADC);
  Serial.println();
  Serial.println();
  delay(1000);
}
