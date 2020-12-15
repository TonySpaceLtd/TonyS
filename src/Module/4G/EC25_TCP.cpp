#include "EC25_TCP.h"

#define RET_STR_NUM 8
static String EC25_RET_STR[RET_STR_NUM]={"OK","ERROR","+QIOPEN:","CONNECT",">","SEND OK","+QIRD:","+QIURC:"};

EC25_TCP::EC25_TCP(uint8_t connectID )
{
	_contextID = 1;
	_connectID = connectID;
	_connected=0;
}
bool EC25_TCP::connect(String ip,int port,uint8_t mode)
{
	_connected = 0;
	bool ret = false;
	//AT+QIOPEN=1,0,"TCP","220.180.239.212",8009,0,0 
	LTE.ECser.sendAT("AT+QIOPEN=",false);
	LTE.ECser.sendAT(String(_contextID),",",false);
	LTE.ECser.sendAT(String(_connectID),",",false);
	LTE.ECser.sendAT("\"TCP\"",",",false);
	LTE.ECser.sendAT("\"",false);
	LTE.ECser.sendAT(ip,false);
	LTE.ECser.sendAT("\"",",",false);
	LTE.ECser.sendAT(String(port),false);
	LTE.ECser.sendAT(",0,",String(mode));
	//String waitstr[3]={"OK","ERROR","CONNECT"};
	//EC_Resp res = LTE.ECser.waitString(waitstr,3,10000);
	//EC_Resp res = LTE.ECser.waitString(F("OK"),10000);
	EC_Resp res = LTE.ECser.waitString(EC25_RET_STR,RET_STR_NUM,10000);
	checkURC(res.id,res.data);
	if(res.id==0)
	{	
		res = LTE.ECser.waitString(EC25_RET_STR,RET_STR_NUM,10000);
		checkURC(res.id,res.data);
		if(res.id==2)
		{
			int index  = res.data.indexOf(",");
			int status = res.data.substring(index+1).toInt();
			//Serial.println(res.data);
			if(status==0)
			{
				ret = true;
				_connected = 1;
			}
			else
			{
				getLastError();
			}				
		}
	}
	else if (res.id==3)
	{
		ret = true;
	}
	else
		getLastError();	
	
	_connectMode = mode;
	return ret;
}
bool EC25_TCP::startSend()
{
	LTE.ECser.sendAT("AT+QISEND=",String(_connectID));
	Serial.println("AT+QISEND="+String(_connectID));
	EC_Resp res = LTE.ECser.waitString(EC25_RET_STR,RET_STR_NUM,3000);
	checkURC(res.id,res.data);
	Serial.println(res.data);
	if(res.id==4)
		return true;
	else
	{	
		int err = getLastError();
		if(err == 572)
		{
			_connected = 0;
		}
	}
	return false;
}
bool EC25_TCP::stopSend()
{
	LTE.ECser.write(0x1A);
	EC_Resp res = LTE.ECser.waitString(EC25_RET_STR,RET_STR_NUM,3000);
	checkURC(res.id,res.data);
	if(res.id==5||res.id==0) // OK or SEND OK
		return true;
	else
	{
		int err = getLastError();	
		if(err == 572)
		{
			_connected = 0;
		}
	}
	return false;
}
int EC25_TCP::writeBytes(const uint8_t *buf, size_t size)
{
	int num=0;
	String str = "AT+QISEND="+String(_connectID)+","+String(size);
	LTE.ECser.sendAT(str);
	EC_Resp res = LTE.ECser.waitString(EC25_RET_STR,RET_STR_NUM,3000);
	if(res.id==4)
	{
		num = LTE.ECser.write(buf,size);
		res = LTE.ECser.waitString(EC25_RET_STR,RET_STR_NUM,3000);
		if(res.id==5||res.id==0) // OK or SEND OK
		{
			return num;
		}
	}
	_connected = 0;
	return 0;
}
int EC25_TCP::print(String str)
{
	return writeBytes((uint8_t*)str.c_str(),str.length());
}
int EC25_TCP::available()
{
	int len=0;
	String str = "AT+QIRD="+String(_connectID)+",0";
	LTE.ECser.sendAT(str);
	//LTE.ECser.sendAT("AT+QIRD=",false);
	//LTE.ECser.sendAT(String(0),",0");
	EC_Resp res = LTE.ECser.waitString(EC25_RET_STR,RET_STR_NUM);
	checkURC(res.id,res.data);
	if(res.id==6)
	{
		int index  = res.data.lastIndexOf(",");
		len = res.data.substring(index+1).toInt();
	}
	res = LTE.ECser.waitString("OK");
	checkURC(res.id,res.data);
	return len;	
}
int  EC25_TCP::read()
{
	uint8_t c;
	readBytes(&c,1);
	return c;
}
int  EC25_TCP::readBytes(uint8_t *buffer,size_t len)
{
	//AT+QIRD=0,1500 
	int lenout = 0;
	String str = "AT+QIRD="+String(_connectID)+","+String(len);
	LTE.ECser.sendAT(str);
	EC_Resp res = LTE.ECser.waitString(EC25_RET_STR,RET_STR_NUM);
	checkURC(res.id,res.data);
	if(res.id==6)
	{
		int index = res.data.indexOf(" ");
		lenout = res.data.substring(index+1).toInt();
		//Serial.print("len: ");Serial.println(len);
	    LTE.ECser.read();
		LTE.ECser.readBytes(buffer,lenout);
		/*for(uint8_t i =0;i<len;i++)
		{
			int c = LTE.ECser.read();
			Serial.print(c,HEX);Serial.print(' ');
		}
		*/
		res = LTE.ECser.waitString("OK");	
		checkURC(res.id,res.data);
	}
	if(lenout==0)
		lenout=-1;
	return lenout;
}

void EC25_TCP:: checkURC(uint8_t n,String data)
{
	if(n==7)
	{
		if(data.indexOf("closed")!=-1)
		{
			int soc = data.substring(data.lastIndexOf(',')+1).toInt();
			
			Serial.print("SocketID:");
			Serial.print(soc);
			Serial.println(" Close");
			_connected =0;
		}
	}
}
uint8_t EC25_TCP:: connected()
{
	return _connected;
}
int EC25_TCP::getLastError()
{
	int errID=0;
	LTE.ECser.sendAT("AT+QIGETERROR");
	EC_Resp res = LTE.ECser.waitString("+QIGETERROR:");
	if(res.id==0)
	{
		Serial.println();
		Serial.print("Error Message: ");
		Serial.println(res.data.substring(res.data.indexOf(" ")+1));
		errID = res.data.substring(res.data.indexOf(' ')+1,res.data.indexOf(',')).toInt();
	}
	res = LTE.ECser.waitString("OK");	
	return errID;
	//+QIGETERROR: 552, invalid parameters
}
bool EC25_TCP::close()
{
	String str = "AT+QICLOSE="+String(_connectID);
	LTE.ECser.sendAT(str);
	EC_Resp res = LTE.ECser.waitString(EC25_RET_STR,RET_STR_NUM);
	checkURC(res.id,res.data);
	if(res.id==0)
	{
		return true;
	}
	return false;
}
void EC25_TCP::flush()
{
	LTE.ECser.flush();
	LTE.ECser.flush();
	LTE.ECser.flush();
}


