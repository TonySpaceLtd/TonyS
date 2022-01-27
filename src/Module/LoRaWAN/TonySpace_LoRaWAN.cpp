#include "TonySpace_LoRaWAN.h"
#include "TonyS_X1.h"

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
TONY_LORA TonyLORA;
extern HardwareSerial LoRaSerial(NULL);
#endif


TONY_LORA::TONY_LORA()
{
}

void TONY_LORA::slot(uint8_t slot) 
{
	LoRaSerial = Tony.SerialBegin(slot,115200);
}

bool TONY_LORA::begin(String deveui, String appeui, String appkey)
{
	//Serial.println("Setting up LoRaWAN module");
	uint16_t timeout = 2000;
	if(deveui != "") 
	{
		if(setDeveui(deveui, timeout) == 0) 
		{
			Serial.println("Set deveui : Fail");
			return 0;
		}
	}
	if(appeui != "") 
	{
		if(setAppeui(appeui, timeout) == 0) 
		{
			Serial.println("Set appeui : Fail");
			return 0;
		}
	}		
	if(appkey != "") 
	{
		if(setAppkey(appkey, timeout) == 0) 
		{
			Serial.println("Set appkey : Fail");
			return 0;
		}
	}
	//Serial.println("Setup is complete.");
	return 1;
}

bool TONY_LORA::reset()
{
	LoRaSerial.print("sip reset");
	delay(1000);
	while(LoRaSerial.available()) LoRaSerial.flush();
	return 1;
}

bool TONY_LORA::getDeveui(char * deveui, uint16_t timeout)
{
	String buff = "";
	LoRaSerial.print("mac get_deveui");
	if(getRespond(deveui, timeout)) return 1;
	else return 0;
}


bool TONY_LORA::getAppeui(char * appeui, uint16_t timeout)
{
	String buff = "";
	LoRaSerial.print("mac get_appeui");
	if(getRespond(appeui, timeout)) return 1;
	else return 0;
}

bool TONY_LORA::getAppkey(char * appkey, uint16_t timeout)
{
	String buff = "";
	LoRaSerial.print("mac get_appkey");
	if(getRespond(appkey, timeout)) return 1;
	else return 0;
}

bool TONY_LORA::joinOTAA(uint16_t timeout)
{
	char buff[7];
	LoRaSerial.print("mac join otaa");
	if(getRespond(buff, timeout)) 
	{
		if(buff == "Ok") return 1;
	}
	else return 0;
}

bool TONY_LORA::joinStatus(uint16_t timeout)
{
	unsigned long startTime = millis(); 
	unsigned long currentTime = 0; 
	String buff;
	while(1)
	{
		buff[0] = '\0';
		LoRaSerial.print("mac get_join_status");
		if(LoRaSerial.available()) buff = LoRaSerial.readString();
		if(buff == ">> joined" || buff.indexOf(">> joined")>0) return 1;	
		currentTime = millis(); 
		if((currentTime-startTime)>=timeout) return 0;	
		delay(100);
	}	
}

bool TONY_LORA::setDeveui(String deveui, uint16_t timeout)
{
	char buff[7];
	String buffstring = "";
	LoRaSerial.print("mac set_deveui "+deveui);
	if(getRespond(buff, timeout)) 
	{
		buffstring = buff;
		if(buffstring == "Ok") return 1;
		else if(buffstring != 0) return 0;
	}
	else return 0;	
}

bool TONY_LORA::setAppeui(String appeui, uint16_t timeout)
{
	char buff[7];
	String buffstring = "";
	LoRaSerial.print("mac set_appeui "+appeui);
	if(getRespond(buff, timeout)) 
	{
		buffstring = buff;
		if(buffstring == "Ok") return 1;
		else if(buffstring != 0) return 0;
	}
	else return 0;	
}


bool TONY_LORA::setAppkey(String appkey, uint16_t timeout)
{ 
	char buff[7];
	String buffstring = "";
	LoRaSerial.print("mac set_appkey "+appkey);
	if(getRespond(buff, timeout)) 
	{
		buffstring = buff;
		if(buffstring == "Ok") return 1;
		else if(buffstring != 0) return 0;
	}
	else return 0;	
}

bool TONY_LORA::stringWrite(uint8_t port, String stringdata, uint16_t timeout)
{
	if(stringdata.length()<=512) // 64 Byte
    {
		char buff[200];
		String buffstring = "";
		port = constrain(port, 0, 223);
		
		if(joinStatus() == 1) LoRaSerial.print("mac tx ucnf "+String(port)+" "+stringdata);
		else
		{
			if(TonyLORA.joinOTAA())
			{			
				if(TonyLORA.joinStatus(timeout))
				{
					LoRaSerial.print("mac tx ucnf "+String(port)+" "+stringdata);
				}
				else return 0;
			}
			else return 0;
		}

		if(getRespond(buff, timeout)) 
		{
			buffstring = buff;
			buffstring.trim();
			if(buffstring == "Ok")
			{
				buff[0] = '\0';
				if(getRespond(buff, timeout)) 
				{
					buffstring = buff;
					buffstring.trim();
					if(buffstring == "tx_ok" || buffstring.indexOf("tx_ok")>0) return 1;    
					else if(buffstring != 0) return 0;
				}
				else return 0;		
			}
			else if(buffstring != 0) return 0;
		}
		else return 0;		
	}
}

bool TONY_LORA::setSyncword(String sync, uint16_t timeout)
{
	char buff[7];
	String buffstring = "";
	LoRaSerial.print("rf set_sync "+sync);
	if(getRespond(buff, timeout)) 
	{
		buffstring = buff;
		if(buffstring == "Ok") return 1;
		else if(buffstring != 0) return 0;
	}
	else return 0;	
}

bool TONY_LORA::receiveBrodcast(char * receivedata, uint16_t timeout)
{
	char buff[40];
	String buffstring = "";
	LoRaSerial.print("rf rx "+String(timeout-200));
	if(getRespond(buff, timeout)) 
	{
		buffstring = buff;
		if(buffstring == "Ok")
		{
			if(getRespond(receivedata, timeout)) return 1;
			else return 0;
		}
		else if(buffstring != 0) return 0;
	}
	else return 0;		
}

bool TONY_LORA::sendBrodcast(String stringdata, uint16_t timeout)
{
	if(stringdata.length()<=2040) // 255 Byte
    {
		char buff[20];
		String buffstring = "";
		LoRaSerial.print("rf tx "+stringdata);
		if(getRespond(buff, timeout)) 
		{
			buffstring = buff;
			if(buffstring == "radio_tx_ok" || buffstring.indexOf("radio_tx_ok")>0) return 1;
			else if(buffstring != 0) return 0;
		}
		else return 0;		
	}
}

bool TONY_LORA::getRespond(char * respond, uint16_t timeout)
{
	unsigned long startTime = millis(); 
	unsigned long currentTime = 0; 
	String buff = "";
	while(1)
	{
		buff = "";
		if(LoRaSerial.available()) buff = LoRaSerial.readString();
		if(buff != "")
		{
			buff = buff.substring(5);
			buff.trim();
			strcpy(respond,&buff[0]);
/* 			Serial.println(buff);
			Serial.print("Length : ");
			Serial.println(buff.length()); */
			return 1;
		}
		
		currentTime = millis(); 
		if((currentTime-startTime)>=timeout) return 0;
	}	
}
