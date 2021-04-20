#include "TonySpace_MAX11301.h"



MAX11301::MAX11301()
{

}

int DAC_Compensate = 0;  //---- For adjust DAC Output


void MAX11301::Command_Config()
{
	//------ Clear GPO Ouput -------//
	Wire.beginTransmission(ADDRMAX11301);
    Wire.write(GPO_port_0_to_15);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.endTransmission();
	delay(1);

	Wire.beginTransmission(ADDRMAX11301);
    Wire.write(GPO_port_16_to_19);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.endTransmission();
	delay(1);
	//------------------------------//

	uint16_t regData = 0;

	regData = Reset;
	regData = (regData<<1)+BRST;
	regData = (regData<<1)+LPEN;
	regData = (regData<<1)+RS_CANCEL;
	regData = (regData<<1)+TMPPER;
	regData = (regData<<3)+TmPCTL;
	regData = (regData<<1)+THSHDN;
	regData = (regData<<1)+DACREF;
	regData = (regData<<2)+ADCconv;
	regData = (regData<<2)+DACCTL;
	regData = (regData<<2)+ADCCTL;

	uint16_t Data_1 = regData >> 8;
    uint8_t Data_2 = regData;
    Wire.beginTransmission(ADDRMAX11301);
    Wire.write(0x10);
    Wire.write(Data_1);
    Wire.write(Data_2);
    Wire.endTransmission();
	delay(1);
	
	Wire.beginTransmission(ADDRMAX11301);  //Interrupt mask register
    Wire.write(0x11);
    Wire.write(0x7f);
    Wire.write(0xc0);
    Wire.endTransmission();
	delay(1);
	
	Wire.beginTransmission(ADDRMAX11301);  //GPI port 0 to 7 mode register
    Wire.write(0x12);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.endTransmission();
	delay(1);
	
	Wire.beginTransmission(ADDRMAX11301);  //GPI port 8 to 15 mode register
    Wire.write(0x13);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.endTransmission();
	delay(1);
	
	Wire.beginTransmission(ADDRMAX11301);  //GPI port 16 to 19 mode register
    Wire.write(0x14);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.endTransmission();
	delay(1);
	
	Wire.beginTransmission(ADDRMAX11301);  //DAC preset data #1
    Wire.write(0x16);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.endTransmission();
	delay(1);
	
	Wire.beginTransmission(ADDRMAX11301);  //DAC preset data #2
    Wire.write(0x17);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.endTransmission();
	delay(1);
	
	Wire.beginTransmission(ADDRMAX11301);  //Temperature monitor configuration
    Wire.write(0x18);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.endTransmission();
	delay(1);
	
	Wire.beginTransmission(ADDRMAX11301);  //Internal temperature monitor high threshold
    Wire.write(0x19);
    Wire.write(0x07);
    Wire.write(0xff);
    Wire.endTransmission();
	delay(1);
	
	Wire.beginTransmission(ADDRMAX11301);  //Internal temperature monitor low threshold
    Wire.write(0x1a);
    Wire.write(0x08);
    Wire.write(0x00);
    Wire.endTransmission();
	delay(1);
	
	Wire.beginTransmission(ADDRMAX11301);  //1st external temperature monitor high threshold
    Wire.write(0x1b);
    Wire.write(0x07);
    Wire.write(0xff);
    Wire.endTransmission();
	delay(1);
	
	Wire.beginTransmission(ADDRMAX11301);  //1st external temperature monitor low threshold
    Wire.write(0x1c);
    Wire.write(0x08);
    Wire.write(0x00);
    Wire.endTransmission();
	delay(1);
	
	Wire.beginTransmission(ADDRMAX11301);  //2nd external temperature monitor high threshold
    Wire.write(0x1d);
    Wire.write(0x07);
    Wire.write(0xff);
    Wire.endTransmission();
	delay(1);
	
	Wire.beginTransmission(ADDRMAX11301);  //2nd external temperature monitor low threshold
    Wire.write(0x1e);
    Wire.write(0x08);
    Wire.write(0x00);
    Wire.endTransmission();
	delay(1);
	
	defaultConfig();
	delay(10);
}

void MAX11301::defaultConfig()
{
	// Config Port
	Basic_Config_Port(0, GPItype);
	delay(1);

	Basic_Config_Port(1, GPItype);
	delay(1);

	Basic_Config_Port(2, GPItype);
	delay(1);

	Basic_Config_Port(3, GPItype);
	delay(1);

	Basic_Config_Port(4, GPItype);
	delay(1);

	Basic_Config_Port(5, GPItype);
	delay(1);

	Basic_Config_Port(6, GPItype);
	delay(1);

	Basic_Config_Port(7, GPItype);
	delay(1);

	Basic_Config_Port(8, GPItype);
	delay(1);

	Basic_Config_Port(9, GPItype);
	delay(1);

	Basic_Config_Port(10, GPItype);
	delay(1);

	Basic_Config_Port(11, GPItype);
	delay(1);
	
	Basic_Config_Port(12, GPItype);
	delay(1);
	
	Basic_Config_Port(13, GPItype);
	delay(1);
	
	Basic_Config_Port(14, GPItype);
	delay(1);
	
	Basic_Config_Port(15, GPItype);
	delay(1);
	
	Basic_Config_Port(16, GPItype);
	delay(1);
	
	Basic_Config_Port(17, GPItype);
	delay(1);
	
	Basic_Config_Port(18, DACADC);
	delay(1);

	Basic_Config_Port(19, GPItype);
	delay(1);
	
	
	// DAC data register
	writeDAC(0, 0);
	delay(1);
	
	writeDAC(1, 0);
	delay(1);
	
	writeDAC(2, 0);
	delay(1);
	
	writeDAC(3, 0);
	delay(1);
	
	writeDAC(4, 0);
	delay(1);
	
	writeDAC(5, 0);
	delay(1);
	
	writeDAC(6, 0);
	delay(1);
	
	writeDAC(7, 0);
	delay(1);
	
	writeDAC(8, 0);
	delay(1);
	
	writeDAC(9, 0);
	delay(1);
	
	writeDAC(10, 0);
	delay(1);
	
	writeDAC(11, 0);
	delay(1);
	
	writeDAC(12, 0);
	delay(1);
	
	writeDAC(13, 0);
	delay(1);
	
	writeDAC(14, 0);
	delay(1);
	
	writeDAC(15, 0);
	delay(1);
	
	writeDAC(16, 0);
	delay(1);
	
	writeDAC(17, 0);
	delay(1);
	
	writeDAC(18, 0);
	delay(1);
	
	writeDAC(19, 0);
	delay(1);
}

bool MAX11301::Config_deviceControl()
{
	delay(1);
	Command_Config();

	//------ Adjust DAC Compensate Value -------//
	uint16_t dataADC = 0;
	writeDAC(18, 184);  // 184 = 0.449V
	for(uint8_t i=0; i<200; i++)
	{
		float Voltage = 0;
		dataADC = readADC(18);  // Read ADC from port 18 (DAC Output)
		Voltage = float(10.0/4095)*dataADC;
		if(Voltage < 0.449)
		{
			DAC_Compensate++;
		}
		else if(Voltage > 0.46)
		{
			DAC_Compensate--;
		}
		else if(Voltage > 0.449 && Voltage <= 0.46)
		{
			Serial.println("Default Config Successfully !");
			//------ End Adjust Compensate Value -----//
			Basic_Config_Port_For_GPI(18, 819);
			Basic_Config_Port_For_GPI(19, 819);
			delay(1);
			return 1;
		}
		writeDAC(18, 184);  // 184 = 0.449V
		
		if(i%40 == 0)
		{
			DAC_Compensate = 0;
			Tony.offExtension();
			delay(200);
			Tony.onExtension();
			delay(200);
			Command_Config();
			Basic_Config_Port_For_DACADC(18, 184);  // Config Port 18 to DAC with monitoring  (184 = 0.449V)
			writeDAC(18, 184);  // 184 = 0.449V
			delay(100);
			//Serial.println("Reset MAX11301");
		}
		delay(10);
		
		if(i >= 199)
		{
			DAC_Compensate = 0;
			Serial.println("Can't calibrate compensate value !");
			//------ End Adjust Compensate Value -----//
			Basic_Config_Port_For_GPI(18, 819);
			Basic_Config_Port_For_GPI(19, 819);
			delay(1);
			return 0;
		}
	}  
}

void MAX11301::Advance_Config_Port(uint8_t Port, byte Mode, bool AVR_INV, byte RANGE, byte SAMPLES, byte ASSOCIATED)
{
	Port -= 100;
	uint16_t regData = 0;
	Port = constrain(Port, 0, 19);

	regData = Mode;
	regData = (regData<<1)+AVR_INV;
	regData = (regData<<3)+RANGE;
	regData = (regData<<3)+SAMPLES;
	regData = (regData<<5)+ASSOCIATED;

	uint16_t Data_1 = regData >> 8;
    uint8_t Data_2 = regData;
    Wire.beginTransmission(ADDRMAX11301);
    Wire.write(Config_Port_00+Port);
    Wire.write(Data_1);
    Wire.write(Data_2);
    Wire.endTransmission();
    // Serial.print("Config Port 0x");
	// Serial.print(Config_Port_00+Port ,HEX);
	// Serial.print("  :  0x");
    // Serial.print(Data_1 ,HEX);
    // Serial.print(Data_2 ,HEX);
    // Serial.println();
	delay(1);
}

void MAX11301::Basic_Config_Port(uint8_t Port, uint16_t Basic_Con)
{
	Port -= 100;
	uint16_t regData = Basic_Con;
	uint16_t Data_1 = regData >> 8;
    uint8_t Data_2 = regData;
	
	Port = constrain(Port, 0, 19);
	
    Wire.beginTransmission(ADDRMAX11301);
    Wire.write(Config_Port_00+Port);
    Wire.write(Data_1);
    Wire.write(Data_2);
    Wire.endTransmission();
    // Serial.print("Config Port 0x");
	// Serial.print(Config_Port_00+Port ,HEX);
	// Serial.print("  :  0x");
    // Serial.print(Data_1 ,HEX);
	// Serial.print("	");
    // Serial.print(Data_2 ,HEX);
    // Serial.println();
	delay(1);
}

void MAX11301::Basic_Config_Port_For_DACADC(uint8_t Port, int16_t Output)
{
	Port -= 100;
	Port = constrain(Port, 0, 19);

	Output = Output+DAC_Compensate;
	Output = constrain(Output, 0, 4095);
	uint16_t regData1 = DACADC;
	uint16_t regData2 = Output;
	uint16_t Data_1 = regData1 >> 8;
    uint8_t Data_2 = regData1;
	uint16_t Data_3 = regData2 >> 8;
    uint8_t Data_4 = regData2;
    Wire.beginTransmission(ADDRMAX11301);
    Wire.write(Config_Port_00+Port);
    Wire.write(Data_1);
    Wire.write(Data_2);
    Wire.endTransmission();
    // Serial.print("Config Port 0x");
	// Serial.print(Config_Port_00+Port ,HEX);
	// Serial.print("  :  0x");
    // Serial.print(Data_1 ,HEX);
	// Serial.print("	");
    // Serial.print(Data_2 ,HEX);
    // Serial.println();
	delay(1);

	Wire.beginTransmission(ADDRMAX11301);
    Wire.write(Config_Port_00+Port+64);
    Wire.write(Data_3);
    Wire.write(Data_4);
    Wire.endTransmission();
    // Serial.print("Config Port 0x");
	// Serial.print(Config_Port_00+Port+64 ,HEX);
	// Serial.print("  :  0x");
    // Serial.print(Data_3 ,HEX);
	// Serial.print("	");
    // Serial.print(Data_4 ,HEX);
    // Serial.println();
	delay(1);
}

void MAX11301::Basic_Config_Port_For_GPI(uint8_t Port, uint16_t Threshold)
{
	Port -= 100;
	Port = constrain(Port, 0, 19);
	Threshold = constrain(Threshold, 0, 4095);

	uint16_t regData1 = GPItype;
	uint16_t regData2 = Threshold;
	uint16_t Data_1 = regData1 >> 8;
    uint8_t Data_2 = regData1;
	uint16_t Data_3 = regData2 >> 8;
    uint8_t Data_4 = regData2;
    Wire.beginTransmission(ADDRMAX11301);
    Wire.write(Config_Port_00+Port);
    Wire.write(Data_1);
    Wire.write(Data_2);
    Wire.endTransmission();
    // Serial.print("Config Port 0x");
	// Serial.print(Config_Port_00+Port ,HEX);
	// Serial.print("  :  0x");
    // Serial.print(Data_1 ,HEX);
	// Serial.print("	");
    // Serial.print(Data_2 ,HEX);
    // Serial.println();
	delay(1);

	Wire.beginTransmission(ADDRMAX11301);
    Wire.write(Config_Port_00+Port+64);
    Wire.write(Data_3);
    Wire.write(Data_4);
    Wire.endTransmission();
    // Serial.print("Config Port 0x");
	// Serial.print(Config_Port_00+Port+64 ,HEX);
	// Serial.print("  :  0x");
    // Serial.print(Data_3 ,HEX);
	// Serial.print("	");
    // Serial.print(Data_4 ,HEX);
    // Serial.println();
	delay(1);
}

void MAX11301::Basic_Config_Port_For_GPO(uint8_t Port, int16_t Output)
{
	Port -= 100;
	Port = constrain(Port, 0, 19);

	Output = Output+DAC_Compensate;
	Output = constrain(Output, 0, 4095);
	
	uint16_t regData1 = GPOtype;
	uint16_t regData2 = Output;
	uint16_t Data_1 = regData1 >> 8;
    uint8_t Data_2 = regData1;
	uint16_t Data_3 = regData2 >> 8;
    uint8_t Data_4 = regData2;
    Wire.beginTransmission(ADDRMAX11301);
    Wire.write(Config_Port_00+Port);
    Wire.write(Data_1);
    Wire.write(Data_2);
    Wire.endTransmission();
    // Serial.print("Config Port 0x");
	// Serial.print(Config_Port_00+Port ,HEX);
	// Serial.print("  :  0x");
    // Serial.print(Data_1 ,HEX);
	// Serial.print("	");
    // Serial.print(Data_2 ,HEX);
    // Serial.println();
	delay(1);

	Wire.beginTransmission(ADDRMAX11301);
    Wire.write(Config_Port_00+Port+64);
    Wire.write(Data_3);
    Wire.write(Data_4);
    Wire.endTransmission();
    // Serial.print("Config Port 0x");
	// Serial.print(Config_Port_00+Port+64 ,HEX);
	// Serial.print("  :  0x");
    // Serial.print(Data_3 ,HEX);
	// Serial.print("	");
    // Serial.print(Data_4 ,HEX);
    // Serial.println();
	delay(1);
}

uint16_t MAX11301::readADC(uint8_t Port)
{
	Port -= 100;
	Port = constrain(Port, 0, 19);
	
	uint8_t getData[2];
	uint16_t dataADC = 0;
	uint8_t count  = 0;

	Wire.beginTransmission(ADDRMAX11301);
	Wire.write(ADC_data_port_00+Port);
	Wire.endTransmission();

	Wire.requestFrom(ADDRMAX11301,2);
	while(Wire.available())
	{
		getData[count] = Wire.read();
		count++;
	}

  dataADC = (getData[0] << 8)+getData[1];
  
  
  if(dataADC >= 5300)
  {
	  dataADC = 0;
	  Serial.println("ADC Fail !");
	  Serial.println();
  }
  else
  {
	return(dataADC);
  }
}

void MAX11301::writeDAC(uint8_t Port, int16_t Output)
{
	Port -= 100;
	int16_t Value = Output+DAC_Compensate;
	Value = constrain(Value, 0, 4095);
    uint16_t Data_1 = Value >> 8;
    uint8_t Data_2 = Value;
	
	Port = constrain(Port, 0, 19);

    Wire.beginTransmission(ADDRMAX11301);
    Wire.write(DAC_data_port_00+Port);
    Wire.write(Data_1);
    Wire.write(Data_2);
    Wire.endTransmission();
}

void MAX11301::writeGPO(uint8_t Port, bool Output)
{
	Port -= 100;
	byte addrPort = 0;
	uint8_t getData[2];
	uint16_t readGPO = 0;
	uint16_t checkGPO = 0;
	uint16_t shiftBit = 1;
	uint8_t count  = 0;
	
	Port = constrain(Port, 0, 19);

    if(Port <= 15)
	{
		addrPort = GPO_port_0_to_15;
	}
	else
	{
		addrPort = GPO_port_16_to_19;
	}
	
	Wire.beginTransmission(ADDRMAX11301);
	Wire.write(addrPort);
	Wire.endTransmission();

	Wire.requestFrom(ADDRMAX11301,2);
	while(Wire.available())
	{
		getData[count] = Wire.read();
		count++;
	}

	readGPO = (getData[0] << 8)+getData[1];

	checkGPO = readGPO;

	if(addrPort == GPO_port_0_to_15)
	{
		shiftBit = shiftBit << Port;
		checkGPO = checkGPO << 15-Port;
		checkGPO = checkGPO >> 15;
	}
	else if(addrPort == GPO_port_16_to_19)
	{
		shiftBit = shiftBit << Port-16;
		checkGPO = checkGPO << 12;
		checkGPO = checkGPO << 19-Port;
		checkGPO = checkGPO >> (15);
	}

	if(checkGPO != Output)
	{
		if(Output == 1)
		{
			readGPO = readGPO | shiftBit;
		}
		else
		{
			readGPO = readGPO ^ shiftBit;
		}
	}

	uint16_t Data_1 = readGPO >> 8;
    uint8_t Data_2 = readGPO;
    Wire.beginTransmission(ADDRMAX11301);
    Wire.write(addrPort);
    Wire.write(Data_1);
    Wire.write(Data_2);
    Wire.endTransmission();
}

void MAX11301::write_speedGPO(uint8_t Port, bool Value)
{	
	Port -= 100;
	byte addrPort = 0;
	if(Value==1)
	{
		output_buf |= 1 << Port; 
	}
	else 
	{
		output_buf &= ~(1 << Port); 
	}
	
	uint8_t Data_1 = 0;
	uint8_t Data_2 = 0;
	Port = constrain(Port, 0, 19);

	if(Port <= 15)
	{
		Data_1 = (output_buf>>8);
		Data_2 = output_buf;
		addrPort = GPO_port_0_to_15;
	}
	else
	{
		Data_1 = 0;
		Data_2 = (output_buf >> 16)&0x0F;
		addrPort = GPO_port_16_to_19;
	}
	
	Wire.beginTransmission(ADDRMAX11301);
	Wire.write(addrPort);
	Wire.write(Data_1);
	Wire.write(Data_2);
	Wire.endTransmission();
}


bool MAX11301::readGPI(uint8_t Port)
{
	Port -= 100;
	byte addrPort = 0;
	uint8_t getData[2];
	uint16_t dataGPI = 0;
	uint8_t count  = 0;
	
	Port = constrain(Port, 0, 19);

    if(Port <= 15)
	{
		addrPort = GPI_port_0_to_15;
	}
	else
	{
		addrPort = GPI_port_16_to_19;
	}

	Wire.beginTransmission(ADDRMAX11301);
	Wire.write(addrPort);
	Wire.endTransmission(); +

	Wire.requestFrom(ADDRMAX11301,2);
	while(Wire.available())
	{
		getData[count] = Wire.read();
		count++;
	}

	dataGPI = (getData[0] << 8)+getData[1];

	if(addrPort == GPI_port_0_to_15)
	{
		dataGPI = dataGPI << 15-Port;
		dataGPI = dataGPI >> 15;
	}
	else if(addrPort == GPI_port_16_to_19)
	{
		dataGPI = dataGPI << 12;
		dataGPI = dataGPI << 19-Port;
		dataGPI = dataGPI >> (15);
	}
	return(dataGPI);
}

bool MAX11301::readGPO(uint8_t Port)
{
	Port -= 100;
	byte addrPort = 0;
	uint8_t getData[2];
	uint16_t dataGPO = 0;
	uint8_t count  = 0;
	
	Port = constrain(Port, 0, 19);

    if(Port <= 15)
	{
		addrPort = GPO_port_0_to_15;
	}
	else
	{
		addrPort = GPO_port_16_to_19;
	}

	Wire.beginTransmission(ADDRMAX11301);
	Wire.write(addrPort);
	Wire.endTransmission(); +

	Wire.requestFrom(ADDRMAX11301,2);
	while(Wire.available())
	{
		getData[count] = Wire.read();
		count++;
	}

	dataGPO = (getData[0] << 8)+getData[1];

	if(addrPort == GPI_port_0_to_15)
	{
		dataGPO = dataGPO << 15-Port;
		dataGPO = dataGPO >> 15;
	}
	else if(addrPort == GPI_port_16_to_19)
	{
		dataGPO = dataGPO << 12;
		dataGPO = dataGPO << 19-Port;
		dataGPO = dataGPO >> (15);
	}
	return(dataGPO);
}