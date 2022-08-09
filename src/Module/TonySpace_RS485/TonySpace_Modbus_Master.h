#ifndef TonySpace_MODBUS_MASTER_h
#define TonySpace_MODBUS_MASTER_h

#include "Arduino.h"
#include "TonySpace_RS485.h"

#define Read_Coil_Status 			0x01
#define Read_Input_Status 			0x02
#define Read_Holding_Register 		0x03
#define Read_Input_Register 		0x04
#define Force_Single_Coil 			0x05
#define Preset_Single_Register 		0x06
#define Force_Multiple_Coils 		0x0F
#define Preset_Multiple_Registers 	0x10


#define SUCCESS 			1
#define MEMORY_FAIL 		2
#define ADDR_FN_FAIL 		3
#define NO_RESPONSE 		4
#define CRC_ERROR			5


class Tony_Modbus_Master 
{
public:
		Tony_Modbus_Master();
		Tony_RS485 ser485;
		void setSlot(uint8_t slot);
		void begin(unsigned long baud,uint32_t config = SERIAL_8N1);
		bool readSingleCoilStatus(uint8_t slaveid,uint16_t address,unsigned long tout=500);
		bool readMultiCoilStatus(uint8_t slaveid,uint16_t address,uint16_t numData,uint8_t **buf,uint16_t *lengh,unsigned long tout=500);
		bool readSingleInputStatus(uint8_t slaveid,uint16_t address,unsigned long tout=500);
		bool readMultiInputStatus(uint8_t slaveid,uint16_t address,uint16_t numData,uint8_t **buf,uint16_t *lengh,unsigned long tout=500);
		
		uint16_t readSingleHoldingRegister(uint8_t slaveid,uint16_t address,unsigned long tout=500);
		bool readMultiHoldingRegister(uint8_t slaveid,uint16_t address,uint16_t numData,uint16_t **buf,uint16_t *lengh,unsigned long tout=500);

		uint16_t readSingleInputRegister(uint8_t slaveid,uint16_t address,unsigned long tout=500);
		bool readMultiInputRegister(uint8_t slaveid,uint16_t address,uint16_t numData,uint16_t **buf,uint16_t *lengh,unsigned long tout=500);

		bool forceSingleCoil(uint8_t slaveid,uint16_t address,bool status,unsigned long tout=500);
		bool presetSingleRegister(uint8_t slaveid,uint16_t address,int data,unsigned long tout=500);
		
		bool forceMultipleCoils(uint8_t slaveid,uint16_t address,uint16_t numberWrite,uint8_t *data,unsigned long tout=500);
		bool presetMultipleRegisters(uint8_t slaveid,uint16_t address,uint16_t numberWrite,uint16_t *data,unsigned long tout=500);

		uint16_t calCRC(uint8_t *data,size_t length);
		void sendRequest(uint8_t slaveID,uint8_t function,uint16_t address,int numData);
		bool readrequeast(uint8_t **buffer,uint16_t *length,unsigned long tout=500);
		
		uint8_t status;
		
private:
		
		uint8_t *data_temp;
		uint8_t slaveID_;
		uint8_t function_;
		uint16_t address_;
		uint16_t numData_;
};

#endif
