#ifndef MAX11300_h
#define MAX11300_h

#include "TonyS_X1.h"
#include "Arduino.h"
#include <SPI.h>

#include "TonySpace_MAX11301.h"

#define MAX11300_CLK 10000000
#define MAX11300_DEV_ID 0x0424

class MAX11300
{
public:
	MAX11300();
	bool Config_deviceControl();
	uint16_t getDeviceID();
	void Command_Config();
	void defaultConfig();
	void Basic_Config_Port(uint8_t Port, uint16_t Basic_Con);
	void Basic_Config_Port_For_DACADC(uint8_t Port, int16_t Output);
	void Basic_Config_Port_For_GPI(uint8_t Port, uint16_t Threshold);
	void Basic_Config_Port_For_GPO(uint8_t Port, int16_t Output);
	uint16_t readADC(uint8_t Port);
	void writeDAC(uint8_t Port, int16_t Output);
	void writeGPO(uint8_t Port, bool Output);
	void write_speedGPO(uint8_t Port, bool Value);
	bool readGPI(uint8_t Port);
	bool readGPO(uint8_t Port);
	uint16_t readReg(uint8_t addr);
	void writeReg8(uint8_t addr, uint8_t dataMSB,uint8_t dataLSB);
	void writeReg(uint8_t addr, uint16_t data);
	void writeReg(uint8_t addr, uint16_t *data,uint8_t len);
	
	
	
private:
	uint32_t output_buf;
	SPISettings recoverySPI;
	uint8_t cs_state;
	bool inTransaction();
	void recoveryTransaction();
};















#endif