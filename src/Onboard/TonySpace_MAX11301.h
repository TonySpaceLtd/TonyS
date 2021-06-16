
#ifndef MAX11301_h
#define MAX11301_h

#include "TonyS_X1.h"
#include "Arduino.h"
#include <Wire.h>

#define ADDRMAX11301    0x38



//------------------------------------ Config Device Control (Start) ------------------------//

#define Reset       0x00    // Self-clearing soft reset register, equivalent to power-on reset.

#define BRST        0x00    // Serial interface burst-mode selection  // 0x00 : Default address incrementing mode.  
																	  // 0x01 : Contextual address incrementing mode.
																	  
#define LPEN        0x00    // Power mode selection  // 0x00 : Default power mode for normal operations  
													 // 0x01 : Lower power mode.
													 
#define RS_CANCEL   0x00    // Temperature sensor series resistor cancellation mode  // 0x00 : Disabled.  
																					 // 0x01 : Enabled.
																					 
#define TMPPER      0x00    // Temperature conversion time control  // 0x00 : Default conversion time setting.  
																	// 0x01 : Extended conversion time setting.
																	
#define TmPCTL      0x00  // Temperature monitor selection  //  0x00 = Disabled Internal temperature monitor , Disabled 1st and 2st external temperature monitor
															//  0x01 = Disabled Internal temperature monitor , Disabled 1st and Enabled 2st external temperature monitor
															//  0x02 = Disabled Internal temperature monitor , Enabled 1st and Disabled 2st external temperature monitor
															//	0x03 = Disabled Internal temperature monitor , Enabled 1st and 2st external temperature monitor
															//	0x04 = Enabled Internal temperature monitor , Disabled 1st and 2st external temperature monitor
															//	0x05 = Enabled Internal temperature monitor , Disabled 1st and Enabled 2st external temperature monitor
															//	0x06 = Enabled Internal temperature monitor , Enabled 1st and Disabled 2st external temperature monitor
															//	0x07 = Enabled Internal temperature monitor , Enabled 1st and 2st external temperature monitor
		
#define THSHDN      0x00    // Thermal shutdown enable  // 0x00 : Thermal shutdown function disabled.  
														// 0x01 : Thermal shutdown function enabled.
														
#define DACREF      0x01    // DAC voltage reference selection  // 0x00 : External reference voltage  
																// 0x01 : Internal reference voltage
																
#define ADCconv     0x03   // ADC conversion rate selection   // 0x00 : ADC conversion rate of 200ksps (default
														      // 0x01 : ADC conversion rate of 250ksps
														      // 0x02 : ADC conversion rate of 333ksps
														      // 0x03 : ADC conversion rate of 400kspsMAX11301PIXI
															  
#define DACCTL      0x00   // DAC mode selection  // 0x00 : Sequential update mode for DAC-configured ports. 
											      // 0x01 : Immediate update mode for DAC-configured ports.
											      // 0x02 : All DAC-configured ports use the same data stored in DACPRSTDAT1[11:0].
											      // 0x03 : All DAC-configured ports use the same data stored in DACPRSTDAT2[11:0].
												  
#define ADCCTL      0x03   // ADC conversion rate selection  // 0x00 : ADC conversion rate of 200ksps (default)
														     // 0x01 : ADC conversion rate of 250ksps
														     // 0x02 : ADC conversion rate of 333ksps
														     // 0x03 : ADC conversion rate of 400ksps
															 
//------------------------------------ Config Device Control (End) ------------------------//          



//-------------- Address Config Port (Start) ----------------//
#define Config_Port_00		0x20
#define Config_Port_01		0x21
#define Config_Port_02		0x22
#define Config_Port_03		0x23
#define Config_Port_04		0x24
#define Config_Port_05		0x25
#define Config_Port_06		0x26
#define Config_Port_07		0x27
#define Config_Port_08		0x28
#define Config_Port_09		0x29
#define Config_Port_10		0x2a
#define Config_Port_11		0x2b
#define Config_Port_12		0x2c
#define Config_Port_13		0x2d
#define Config_Port_14		0x2e
#define Config_Port_15		0x2f
#define Config_Port_16		0x30
#define Config_Port_17		0x31
#define Config_Port_18		0x32
#define Config_Port_19		0x33


//------------------------------------- Basic Config Port (Start) -------------------------//  

#define  ADCtype1		    0x71e0  // Rang ADC 0-10V , DAC 0-10V , Samples 128 , Internal Referent
#define  ADCtype2		    0x76e0  // Rang ADC 0-2.5V , DAC 0-10V , Samples 128 , Internal Referent
#define  DACtype		    0x5100  // Rang DAC 0-10V , Internal Referent
#define  ADC10V  			ADCtype1
#define  ADC2V5			    ADCtype2

//------ For GPO -----
#define  GPOtype		    0x3000  // Config_Port_xx

//------ For GPI -----
#define  GPItype		    0x1000  // Config_Port_xx

//------ For DAC with ADC monitoring -------
#define  DACADC             0x6100



//-------------------------------------- Basic Config Port (END) --------------------------//   


//------------------------------------ Advance Config Port (Start) ------------------------//    

//#define Mode      0x07    // Mode selection   // 0x00 : Mode 0 =  High impedance
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



//---------- Address Read & Write Port ------------//
#define ADC_data_port_00	0x40
#define ADC_data_port_01	0x41
#define ADC_data_port_02	0x42
#define ADC_data_port_03	0x43
#define ADC_data_port_04	0x44
#define ADC_data_port_05	0x45
#define ADC_data_port_06	0x46
#define ADC_data_port_07	0x47
#define ADC_data_port_08	0x48
#define ADC_data_port_09	0x49
#define ADC_data_port_10	0x4A
#define ADC_data_port_11	0x4B
#define ADC_data_port_12	0x4C
#define ADC_data_port_13	0x4D
#define ADC_data_port_14	0x4E
#define ADC_data_port_15	0x4F
#define ADC_data_port_16	0x50
#define ADC_data_port_17	0x51	
#define ADC_data_port_18	0x52
#define ADC_data_port_19	0x53

#define DAC_data_port_00	0x60
#define DAC_data_port_01	0x61
#define DAC_data_port_02	0x62
#define DAC_data_port_03	0x63
#define DAC_data_port_04	0x64
#define DAC_data_port_05	0x65
#define DAC_data_port_06	0x66
#define DAC_data_port_07	0x67
#define DAC_data_port_08	0x68
#define DAC_data_port_09	0x69
#define DAC_data_port_10	0x6A
#define DAC_data_port_11	0x6B
#define DAC_data_port_12	0x6C
#define DAC_data_port_13	0x6D
#define DAC_data_port_14	0x6E
#define DAC_data_port_15	0x6F
#define DAC_data_port_16	0x70
#define DAC_data_port_17	0x71	
#define DAC_data_port_18	0x72
#define DAC_data_port_19	0x73

#define GPO_port_0_to_15	0x0d
#define GPO_port_16_to_19	0x0e

#define GPI_port_0_to_15	0x0b
#define GPI_port_16_to_19	0x0c
//------------------------------------------//


class MAX11301
{
public:
	MAX11301();
	void Command_Config();
	bool Config_deviceControl();
	void Advance_Config_Port(uint8_t Port, byte Mode, bool AVR_INV, byte RANGE, byte SAMPLES, byte ASSOCIATED);
	void Basic_Config_Port(uint8_t Port, uint16_t Basic_Con);
	void Basic_Config_Port_For_DACADC(uint8_t Port, int16_t Output); // DAC with ADC monitoring
	void Basic_Config_Port_For_GPI(uint8_t Port, uint16_t Threshold);
	void Basic_Config_Port_For_GPO(uint8_t Port, int16_t Output);
    uint16_t readADC(uint8_t Port);
	void writeDAC(uint8_t Port, int16_t Output);
	void writeGPO(uint8_t Port, bool Output);
	void write_speedGPO(uint8_t Port, bool Value);
	bool readGPI(uint8_t Port);
	bool readGPO(uint8_t Port);
	void defaultConfig();

private:
	uint32_t output_buf;
};


#endif





















