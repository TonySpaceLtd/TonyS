#include "TonySpace_MAX11300.h"

static int DAC_Compensate = 0;  //---- For adjust DAC Output

MAX11300::MAX11300()
{
	
}

bool MAX11300::Config_deviceControl()
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
	return 0;	
	
}
void MAX11300::Command_Config()
{
	
	writeReg(GPO_port_0_to_15,0x0101);
	writeReg(GPO_port_16_to_19,0x0101);
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
	
	writeReg8(0x10, Data_1,Data_2);
	writeReg8(0x11, 0x7f,0xc0);
	writeReg8(0x12, 0x00,0x00);
	writeReg8(0x13, 0x00,0x00);
	writeReg8(0x14, 0x00,0x00);
	writeReg8(0x16, 0x00,0x00);
	writeReg8(0x17, 0x00,0x00);
	writeReg8(0x18, 0x00,0x00);
	writeReg8(0x19, 0x07,0xff);
	writeReg8(0x1A, 0x08,0x00);
	writeReg8(0x1b, 0x07,0xff);
	writeReg8(0x1c, 0x08,0x00);
	writeReg8(0x1d, 0x07,0xff);
	writeReg8(0x1e, 0x08,0x00);
	defaultConfig();
	
	
	
	
	
}
void MAX11300::Basic_Config_Port(uint8_t Port, uint16_t Basic_Con)
{
	Port -= 100;
	uint16_t regData = Basic_Con;
	uint16_t Data_1 = regData >> 8;
    uint8_t Data_2 = regData;
	Port = constrain(Port, 0, 19);
	writeReg8(Config_Port_00+Port, Data_1,Data_2);
}
void MAX11300::defaultConfig()
{
	for(uint8_t i=0;i<20;i++)
	{
	  Basic_Config_Port(i, GPItype);
	  writeDAC(i, 0);
	}
	
	
}
uint16_t MAX11300::getDeviceID()
{
	return readReg(0x00);
}
void MAX11300::Basic_Config_Port_For_DACADC(uint8_t Port, int16_t Output)
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
    
	writeReg8(Config_Port_00+Port, Data_1,Data_2);
	
    // Serial.print("Config Port 0x");
	// Serial.print(Config_Port_00+Port ,HEX);
	// Serial.print("  :  0x");
    // Serial.print(Data_1 ,HEX);
	// Serial.print("	");
    // Serial.print(Data_2 ,HEX);
    // Serial.println();
	delay(1);
	writeReg8(Config_Port_00+Port+64, Data_3,Data_4);
	
    // Serial.print("Config Port 0x");
	// Serial.print(Config_Port_00+Port+64 ,HEX);
	// Serial.print("  :  0x");
    // Serial.print(Data_3 ,HEX);
	// Serial.print("	");
    // Serial.print(Data_4 ,HEX);
    // Serial.println();
	delay(1);
}

void MAX11300::Basic_Config_Port_For_GPI(uint8_t Port, uint16_t Threshold)
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
	writeReg8(Config_Port_00+Port, Data_1,Data_2);
   
    // Serial.print("Config Port 0x");
	// Serial.print(Config_Port_00+Port ,HEX);
	// Serial.print("  :  0x");
    // Serial.print(Data_1 ,HEX);
	// Serial.print("	");
    // Serial.print(Data_2 ,HEX);
    // Serial.println();
	delay(1);

	writeReg8(Config_Port_00+Port+64, Data_3,Data_4);
    // Serial.print("Config Port 0x");
	// Serial.print(Config_Port_00+Port+64 ,HEX);
	// Serial.print("  :  0x");
    // Serial.print(Data_3 ,HEX);
	// Serial.print("	");
    // Serial.print(Data_4 ,HEX);
    // Serial.println();
	delay(1);
}

void MAX11300::Basic_Config_Port_For_GPO(uint8_t Port, int16_t Output)
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
	writeReg8(Config_Port_00+Port, Data_1,Data_2);
   
    // Serial.print("Config Port 0x");
	// Serial.print(Config_Port_00+Port ,HEX);
	// Serial.print("  :  0x");
    // Serial.print(Data_1 ,HEX);
	// Serial.print("	");
    // Serial.print(Data_2 ,HEX);
    // Serial.println();
	delay(1);
    writeReg8(Config_Port_00+Port+64, Data_3,Data_4);
    // Serial.print("Config Port 0x");
	// Serial.print(Config_Port_00+Port+64 ,HEX);
	// Serial.print("  :  0x");
    // Serial.print(Data_3 ,HEX);
	// Serial.print("	");
    // Serial.print(Data_4 ,HEX);
    // Serial.println();
	delay(1);
}


uint16_t MAX11300::readADC(uint8_t Port)
{
	Port -= 100;
	Port = constrain(Port, 0, 19);
	
	uint8_t getData[2];
	uint16_t dataADC = 0;
	uint8_t count  = 0;
	dataADC = readReg(ADC_data_port_00+Port);
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
	return(dataADC);
}
void MAX11300::writeDAC(uint8_t Port, int16_t Output)
{
	Port -= 100;
	int16_t Value = Output+DAC_Compensate;
	Value = constrain(Value, 0, 4095);
    uint16_t Data_1 = Value >> 8;
    uint8_t Data_2 = Value;
	Port = constrain(Port, 0, 19);
    writeReg8(DAC_data_port_00+Port, Data_1,Data_2);
}

void MAX11300::writeGPO(uint8_t Port, bool Output)
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
	
	
	readGPO = readReg(addrPort);
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
    writeReg8(addrPort, Data_1,Data_2);
	
}
void MAX11300::write_speedGPO(uint8_t Port, bool Value)
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
	 writeReg8(addrPort, Data_1,Data_2);
}

bool MAX11300::readGPI(uint8_t Port)
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
	dataGPI = readReg(addrPort);
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
bool MAX11300::readGPO(uint8_t Port)
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

	dataGPO = readReg(addrPort);
	

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

void MAX11300::writeReg8(uint8_t addr, uint8_t dataMSB,uint8_t dataLSB)
{
	
	uint8_t pvCS = digitalRead(IO_CS);
		digitalWrite(IO_CS,HIGH);
	
	//inTransaction();
	SPI.endTransaction();	
	SPI.beginTransaction(SPISettings(MAX11300_CLK, MSBFIRST, SPI_MODE0));
	digitalWrite(SEL_CS_MAX3100,LOW);
    SPI.transfer(addr<<1);
    SPI.transfer(dataMSB);
    SPI.transfer(dataLSB);
    SPI.endTransaction();	
	digitalWrite(SEL_CS_MAX3100,HIGH);
	digitalWrite(IO_CS,pvCS);
	//recoveryTransaction();
}
void MAX11300::writeReg(uint8_t addr, uint16_t data)
{
	writeReg(addr,&data,1);
}
void MAX11300::writeReg(uint8_t addr, uint16_t *data,uint8_t len)

{//#define MAX113XXAddr_SPI_Write(RegAddr) ( (RegAddr << 1)    

	uint8_t pvCS = digitalRead(IO_CS);
	digitalWrite(IO_CS,HIGH);
	SPI.endTransaction();
	SPI.beginTransaction(SPISettings(MAX11300_CLK, MSBFIRST, SPI_MODE0));
	digitalWrite(SEL_CS_MAX3100,LOW);	
	
    SPI.transfer(addr<<1);
	 for(uint8_t idx = 0; idx < len; idx++)
    {
        SPI.transfer(((0xFF00 & data[idx]) >> 8));
        SPI.transfer((0x00FF & data[idx]));
    }
	
    SPI.endTransaction();
	digitalWrite(SEL_CS_MAX3100,HIGH);
	digitalWrite(IO_CS,pvCS);
}
uint16_t MAX11300::readReg(uint8_t addr)
{
	uint8_t pvCS = digitalRead(IO_CS);
		digitalWrite(IO_CS,HIGH);
	SPI.endTransaction();
	SPI.beginTransaction(SPISettings(MAX11300_CLK, MSBFIRST, SPI_MODE0));
	digitalWrite(SEL_CS_MAX3100,LOW);	
	uint8_t data[3]={(addr<<1)|1,0xFF,0xFF};
    SPI.transfer(data,3);
    SPI.endTransaction();
	digitalWrite(SEL_CS_MAX3100,HIGH);
	digitalWrite(IO_CS,pvCS);
	uint16_t ret = data[1]<<8|data[2];
	return  ret;
}
bool MAX11300::inTransaction()
{
	cs_state = digitalRead(IO_CS);
	if(cs_state==0)
	{
		spi_t   *spiDev = SPI.bus();
		uint32_t  spiFreq = spiClockDivToFrequency(spiGetClockDiv(spiDev));
		uint8_t spiDataMode = spiGetDataMode(spiDev);
		uint8_t spiBitOrder = spiGetBitOrder(spiDev);
		Serial.printf("Recovery: %d %d %d\r\n",spiFreq,spiBitOrder,spiDataMode);
		
		recoverySPI = SPISettings(spiFreq,spiBitOrder,spiDataMode);
	}
	digitalWrite(IO_CS,HIGH);
	SPI.endTransaction();
}
void MAX11300::recoveryTransaction()
{
	if(cs_state==0)
	{
		
		SPI.beginTransaction(recoverySPI);
		digitalWrite(IO_CS,cs_state);
	}
}














