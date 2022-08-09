
#include "TonySpace_Modbus_Master.h"

Tony_Modbus_Master::Tony_Modbus_Master()
{
	
}
void Tony_Modbus_Master::setSlot(uint8_t slot)
{
	ser485.setSlot(slot);
}
void Tony_Modbus_Master::begin(unsigned long baud,uint32_t config )
{
	ser485.begin(baud,config);
}
bool Tony_Modbus_Master::readSingleCoilStatus(uint8_t slaveid,uint16_t address,unsigned long tout)
{
	uint8_t *buf;
	uint16_t len;
	sendRequest(slaveid,Read_Coil_Status,address,1);
	if(readrequeast(&buf,&len,tout))
	{
		if(buf[0])
			return true;
	}
	else
		Serial.println("Error");
	return false;
}
bool Tony_Modbus_Master::readMultiCoilStatus(uint8_t slaveid,uint16_t address,uint16_t numData,uint8_t **buf,uint16_t *lengh,unsigned long tout)
{
	sendRequest(slaveid,Read_Coil_Status,address,numData);
	if(readrequeast(buf,lengh,tout))
	{
		return true;
	}
	return false;
}
bool Tony_Modbus_Master::readSingleInputStatus(uint8_t slaveid,uint16_t address,unsigned long tout)
{
	uint8_t *buf;
	uint16_t len;
	sendRequest(slaveid,Read_Input_Status,address,1);
	if(readrequeast(&buf,&len,tout))
	{
		if(buf[0])
			return true;
	}
	else
		Serial.println("Error");
	return false;
}
bool Tony_Modbus_Master::readMultiInputStatus(uint8_t slaveid,uint16_t address,uint16_t numData,uint8_t **buf,uint16_t *lengh,unsigned long tout)
{
	sendRequest(slaveid,Read_Input_Status,address,numData);
	if(readrequeast(buf,lengh,tout))
	{
		return true;
	}
	return false;
}

uint16_t Tony_Modbus_Master::readSingleHoldingRegister(uint8_t slaveid,uint16_t address,unsigned long tout)
{
	uint16_t ret=0;
	uint8_t *buf;
	uint16_t len;
	sendRequest(slaveid,Read_Holding_Register,address,1);
	if(readrequeast(&buf,&len,tout))
	{
		ret = buf[0]<<8;
		ret |= buf[1];
	}
	return ret;
}
bool Tony_Modbus_Master::readMultiHoldingRegister(uint8_t slaveid,uint16_t address,uint16_t numData,uint16_t **buf,uint16_t *lengh,unsigned long tout)
{
	bool ret=false;
	uint8_t *buff;
	uint16_t len;
	sendRequest(slaveid,Read_Holding_Register,address,numData);
	if(readrequeast(&buff,&len,tout))
	{
		for(uint16_t i=0;i<len;i+=2)
		{
			uint8_t b = buff[i];
			buff[i] = buff[i+1];
			buff[i+1] = b;
		}
		*buf = (uint16_t*)buff;
		lengh[0] = len/2;
		ret=true;
	}
	return ret;
}

uint16_t Tony_Modbus_Master::readSingleInputRegister(uint8_t slaveid,uint16_t address,unsigned long tout)
{
	uint16_t ret=0;
	uint8_t *buf;
	uint16_t len;
	sendRequest(slaveid,Read_Input_Register,address,1);
	if(readrequeast(&buf,&len,tout))
	{
		ret = buf[0]<<8;
		ret |= buf[1];
	}
	return ret;
}

bool Tony_Modbus_Master::readMultiInputRegister(uint8_t slaveid,uint16_t address,uint16_t numData,uint16_t **buf,uint16_t *lengh,unsigned long tout)
{
	bool ret=false;
	uint8_t *buff;
	uint16_t len;
	sendRequest(slaveid,Read_Input_Register,address,numData);
	if(readrequeast(&buff,&len,tout))
	{
		for(uint16_t i=0;i<len;i+=2)
		{
			uint8_t b = buff[i];
			buff[i] = buff[i+1];
			buff[i+1] = b;
		}
		*buf = (uint16_t*)buff;
		lengh[0] = len/2;
		ret=true;
	}
	return ret;
}

bool Tony_Modbus_Master::forceSingleCoil(uint8_t slaveid,uint16_t address,bool status,unsigned long tout)
{
	uint16_t w=0;
	uint8_t *buff;
	uint16_t len;
	
	if(status)
		w = 0xFF00;
	sendRequest(slaveid,Force_Single_Coil,address,w);
	
	if(readrequeast(&buff,&len,tout))
	{
		return true;
	}
	return false;
}
bool Tony_Modbus_Master::presetSingleRegister(uint8_t slaveid,uint16_t address,int data,unsigned long tout)
{
	uint8_t *buff;
	uint16_t len;
	sendRequest(slaveid,Preset_Single_Register,address,data);
	
	if(readrequeast(&buff,&len,tout))
	{
		Serial.println("OK");
		return true;
	}
	Serial.println("Fail");
	return false;
}
bool Tony_Modbus_Master::forceMultipleCoils(uint8_t slaveid,uint16_t address,uint16_t numberWrite,uint8_t *data,unsigned long tout)
{
	slaveID_ = slaveid;
	function_ = Force_Multiple_Coils;
	address_ = address;
	
	uint16_t nWrite = numberWrite/8;
	if(numberWrite%8>0)
		nWrite+=1;
	
	uint8_t *buff;
	if(psramFound())
	{
		buff = (uint8_t *)ps_calloc(nWrite+9, sizeof(uint8_t) );		
	}
	else
	{
		buff = (uint8_t *)calloc(nWrite+9, sizeof(uint8_t) );
	}
	if(buff==NULL)
	{
		status=MEMORY_FAIL;
		return (false);
	}
	size_t cnt=0;
	buff[cnt++] = slaveID_;
	buff[cnt++] = function_;
	buff[cnt++] = address_>>8;
	buff[cnt++] = address_&0x00FF;
	buff[cnt++] = numberWrite>>8;
	buff[cnt++] = numberWrite&0x00FF;
	buff[cnt++] = nWrite&0x00FF;
	for(uint16_t i=0;i<nWrite;i++)
	{
		buff[cnt++] = data[i];
	}
	uint16_t crc = calCRC(buff,cnt);
	buff[cnt++] = crc&0x00FF;
	buff[cnt++] = crc>>8;
	ser485.write(buff,cnt);
	free(buff);
	
	uint8_t *buff2;
	uint16_t len;
	if(readrequeast(&buff2,&len,tout))
	{
		//Serial.println("OK");
		return true;
	}
	//Serial.println("Fail");
	
	return false;
}

bool Tony_Modbus_Master::presetMultipleRegisters(uint8_t slaveid,uint16_t address,uint16_t numberWrite,uint16_t *data,unsigned long tout)
{
	slaveID_ = slaveid;
	function_ = Preset_Multiple_Registers;
	address_ = address;
	uint16_t nWrite = numberWrite*2;
	
	uint8_t *buff;
	if(psramFound())
	{
		buff = (uint8_t *)ps_calloc(nWrite+9, sizeof(uint8_t) );		
	}
	else
	{
		buff = (uint8_t *)calloc(nWrite+9, sizeof(uint8_t) );
	}
	if(buff==NULL)
	{
		status=MEMORY_FAIL;
		return (false);
	}
	
	size_t cnt=0;
	buff[cnt++] = slaveID_;
	buff[cnt++] = function_;
	buff[cnt++] = address_>>8;
	buff[cnt++] = address_&0x00FF;
	buff[cnt++] = numberWrite>>8;
	buff[cnt++] = numberWrite&0x00FF;
	buff[cnt++] = nWrite&0x00FF;
	for(uint16_t i=0;i<numberWrite;i++)
	{
		buff[cnt++] = (data[i]>>8)&0x00FF;
		buff[cnt++] = data[i]&0x00FF;
	}
	uint16_t crc = calCRC(buff,cnt);
	buff[cnt++] = crc&0x00FF;
	buff[cnt++] = crc>>8;
	ser485.write(buff,cnt);
	free(buff);
	uint8_t *buff2;
	uint16_t len;
	if(readrequeast(&buff2,&len,tout))
	{
		//Serial.println("OK");
		return true;
	}
	//Serial.println("Fail");
	
return false;	
}

uint16_t Tony_Modbus_Master::calCRC(uint8_t *data,size_t length)
{
	uint16_t crc = 0xFFFF;
	
	 for (size_t position = 0; position < length; position++)
    {
        uint8_t d = data[position]&0xFF;
        crc ^= d;
        for (uint8_t i = 8; i != 0; i--)
        {   
			if ((crc & 0x0001) != 0) 
			{
                crc >>= 1;
                crc ^= 0xA001;
            }
            else          
                crc >>= 1;
        }
    }
	return crc;
}	
void Tony_Modbus_Master::sendRequest(uint8_t slaveID,uint8_t function,uint16_t address,int numData)
{
	slaveID_ = slaveID;
	function_ = function;
	address_ = address;
	numData_ = numData;
	
	uint8_t buff[8];
	size_t cnt=0;
	buff[cnt++] = slaveID_;
	buff[cnt++] = function_;
	buff[cnt++] = address_>>8;
	buff[cnt++] = address_&0x00FF;
	buff[cnt++] = numData_>>8;
	buff[cnt++] = numData_&0x00FF;
	uint16_t crc = calCRC(buff,cnt);
	buff[cnt++] = crc&0x00FF;
	buff[cnt++] = crc>>8;
	ser485.write(buff,cnt);

}


bool Tony_Modbus_Master::readrequeast(uint8_t **buffer,uint16_t *length,unsigned long tout)
{
	unsigned long pv;
	unsigned long t_out = tout;
	size_t rx_max = 5;
	size_t cnt=0;
	
	if((function_ == Read_Coil_Status)||(function_ == Read_Input_Status))
	{
		rx_max += numData_/8;
		if(numData_%8>0)
			rx_max+=1;
		
	}
	else if((function_ == Force_Single_Coil)||(function_ == Preset_Single_Register)||(function_ == Force_Multiple_Coils)||(function_ == Preset_Multiple_Registers))
	{
		rx_max = 8;
	}
	else 
		rx_max += (numData_*2);
	
	bool flag=false;
	
	if(data_temp!=NULL)
		free(data_temp);
		
	if(psramFound())
	{
		data_temp = (uint8_t *)ps_calloc(rx_max, sizeof(uint8_t) );		
	}
	else
	{
		data_temp = (uint8_t *)calloc(rx_max, sizeof(uint8_t) );
	}

	if(data_temp==NULL)
	{
		status=MEMORY_FAIL;
		return (false);
	}
	
	uint8_t *buff = data_temp;
	
	pv = millis();
	do
	{
		if(ser485.available())
		{
			uint8_t d = ser485.read();
			//Serial.print(d,HEX);
			//Serial.print(' ');
			if(flag == false)
			{
				if(d == slaveID_)
				{
					bool flagfn=true;
					uint8_t n=0;
					while(flagfn)
					{
						if(ser485.available())
							flagfn=false;
						delay(1);
						n++;
						if(n>=10)
							flagfn=false;
					}
					if(ser485.peek()==function_)
						flag = true;
				}	
			}
			if(flag == true)
			{
				buff[cnt++] = d;
					
				if(cnt>=rx_max)
				{
					uint16_t 	crc_rx = buff[cnt-1]<<8;
								crc_rx |= buff[cnt-2]&0x00FF;
					uint16_t 	crc = calCRC(buff,(cnt-2));
					if(crc_rx == crc)
					{
						//Serial.println("Data OK");
						/*res.status = true;
						res.id = buff[0];
						res.function = buff[1];
						res.length = buff[2];
						res.data = buff+3;
						*/
						//memcpy(res.data,buff+3,buff[2]); 
						length[0] =  buff[2];
						*buffer	= buff+3;
						
						status = SUCCESS;
						return true;
					}
					else
					{
						Serial.println("CRC_ERROR");
						status = CRC_ERROR;
						return false;
					}
					
				}
			}
			
			pv = millis();
			t_out = 200;
			
		}
		
	}
	while((millis()-pv)<t_out);
	//Serial.println("OUT___"+String(cnt));
	if(flag==false)
		status =  ADDR_FN_FAIL;
	else
		status = NO_RESPONSE;
	return false;
}
