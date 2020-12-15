// V.TS001
#include "TonySpace_FRAM.h"
#include "TonyS_X1.h"


#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
TONY_FRAM TonyFRAM;
#endif


TONY_FRAM::TONY_FRAM()
{
}


bool TONY_FRAM::write(uint16_t addrData , uint32_t dataFRAM)
{
	Serial.println("\r\n");
	Serial.println("Writing to FRAM...");
	Serial.print("Write to address : ");
	Serial.print(addrData);
	Serial.println(" (DEC)");
	
	uint8_t addr = 0x00;
	uint16_t calAddr = 0x00;
	unsigned long writeData = 0x00;

	calAddr = addrData;
	if(addrData <= 255)
	{
		addr = addrFRAM_P1;
	}
	else
	{
		addr = addrFRAM_P2;
		calAddr = calAddr-256;		
	}
	
	// ---------------  Select Address for write data ----------------//
	Wire.beginTransmission(addr);
	Wire.write(calAddr);
	// -------------------------------------------- ----------------//
	
	// ------------------------ Write Data -------------------------//	
	Serial.print("Data : ");
	Serial.print(dataFRAM);
	Serial.println(" (DEC)");

	
	
	//-------------  Data 1 ------------//
	writeData = dataFRAM >> 24;
	Wire.write(writeData);
	writeData = dataFRAM << 8;
	writeData = writeData >> 24;
	Wire.write(writeData);
	writeData = dataFRAM << 16;
	writeData = writeData >> 24;
	Wire.write(writeData);
	writeData = dataFRAM << 24;
	writeData = writeData >> 24;
	Wire.write(writeData);
	
	Wire.endTransmission();  
	
	
	Serial.println("Successfully");
	return(1);

	// -------------------------------------------------------------//
}


uint32_t TONY_FRAM::read(uint16_t addrData)
{    
	uint8_t addr = 0x00;
	uint16_t calAddr = 0x00;
	uint32_t readData = 0x00;
	
	Serial.print("\r\n");
	Serial.println("Reading from FRAM...");
	Serial.print("Read from address : ");
	Serial.print(addrData);
	Serial.println(" (DEC)");
	
	calAddr = addrData;
	if(addrData <= 255)
	{
		addr = addrFRAM_P1;
	}
	else
	{
		addr = addrFRAM_P2;
		calAddr = calAddr-256;		
	}
	
	// ---------------  Select Address for Read data ----------------//
	Wire.beginTransmission(addr);
	Wire.write(calAddr);
	Wire.endTransmission(); 
	// -------------------------------------------- ----------------//
	
	// ------------------------- Read Data -------------------------//
	
	uint8_t getData[4];
	uint8_t count  =0;

	Wire.requestFrom(addr,4);
	while(Wire.available())
	{
		getData[count] = Wire.read();
		count++;
	}

	for(uint8_t i=0;i<4;i++)
	{
		Serial.print(getData[i],HEX);
		Serial.print(" ");
	}

	readData = (getData[0] << 8) + getData[1];
	readData = (readData << 8) + getData[2];
	readData = (readData << 8) + getData[3];

	Serial.println();
	Serial.print("Data : ");
	Serial.print(readData);
	Serial.println(" (DEC)");

	return(readData);

}

uint16_t TONY_FRAM::size(uint8_t step)
{
	uint8_t addr = 0x00;
	uint16_t calAddr = 0;
	uint16_t  maxAddress = 511;
	uint32_t readData = 0x00;
	
	Serial.print("\r\n");
	Serial.println("Checking size of FRAM...");
	
	// ------------------------- Read Data -------------------------//
	
	for(uint16_t i=0; i<maxAddress; i=i+step) 
	{
		calAddr = i;
		if(i <= 255)
		{
			addr = addrFRAM_P1;
		}
		else
		{
			addr = addrFRAM_P2;
			calAddr = calAddr-256;		
		}
		
		Wire.beginTransmission(addr);
		Wire.write(calAddr);
		Wire.endTransmission(); 
	
		uint8_t getData[4];
		uint8_t count  =0;
	
		Wire.requestFrom(addr,4);   //------  Request 4 byte for clear buffer 
		while(Wire.available())
		{
			getData[count] = Wire.read();
			count++;
		}
		delay(10);
		
		if(getData[0] == 0)        //------- Use first byte for check data
		{
			Serial.print("Size of FRAM : ");
			Serial.print(maxAddress+1);
			Serial.print("Byte  (0 to ");
			Serial.print(maxAddress);
			Serial.println(")");
			Serial.print("Blank Address start from : ");
			Serial.print(i);
			Serial.println(" (DEC)");
			return(i);
			break;
		}
		else if(i >= maxAddress - step)
		{
			Serial.println("Data Full");
			return(maxAddress+1);  //--- Data full
		}			
	}
	// -------------------------------------------------------------//
}

bool TONY_FRAM::clear()
{
	uint8_t addr = 0x00;
	uint16_t calAddr = 0;
	uint16_t  maxAddress = 511;
	uint8_t  calFinish = 0;
	uint8_t  bar = 0;
	// ---------------------- Clear Data FRAM -----------------------//
	Serial.println("\r\n");
	Serial.println("Start clear data FRAM...\r\n");
	
	Serial.println("0%|--------------------|100%");
	Serial.print("   ");
	
	for(uint16_t i=0; i<maxAddress; i=i+8) 
	{
		calAddr = i;
		if(i <= 255)
		{
			addr = addrFRAM_P1;
		}
		else
		{
			addr = addrFRAM_P2;
			calAddr = calAddr-256;		
		}
	
		Wire.beginTransmission(addr);
		Wire.write(calAddr);

		Wire.write(0x00);
		Wire.write(0x00);
		Wire.write(0x00);
		Wire.write(0x00);
		Wire.write(0x00);
		Wire.write(0x00);
		Wire.write(0x00);
		Wire.write(0x00);
		
		Wire.endTransmission();  
		calFinish = (i*100)/maxAddress;

		if(bar < calFinish) {			
			Serial.print("#");
			bar = bar+5;
		}
		delay(100);
	}
	
	Serial.println("|100 % Complete !");
	Serial.println("\r\n");
	// -------------------------------------------------------------//
}
