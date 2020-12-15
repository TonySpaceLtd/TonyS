#include "EC25_SSL.h"

#define RET_STR_NUM 8
static String EC25_RET_STR[RET_STR_NUM]={"OK","ERROR","+QSSLOPEN:","CONNECT",">","SEND OK","+QIRD:","+QIURC:"};

EC25_SSL::EC25_SSL(uint8_t connectID)
{
	_pdpctxID  = 1;  			//1-16
	_sslctxID  = 1;  			//0-5
	_clientID  = connectID;  	//0-11
	_connected = 0;
}
void EC25_SSL::init()
{
	EC_Resp res;
	LTE.ECser.sendAT("AT+QSSLCFG=\"sslversion\",1,1");
	res = LTE.ECser.waitString("OK");
	LTE.ECser.sendAT("AT+QSSLCFG=\"ciphersuite\",1,0XFFFF");
	res = LTE.ECser.waitString("OK");
	LTE.ECser.sendAT("AT+QSSLCFG=\"seclevel\",1,0");
	res = LTE.ECser.waitString("OK");
	//LTE.ECser.sendAT("AT+QSSLCFG=\"cacert\",1,\"RAM:cacert.pem\"");
	//res = LTE.ECser.waitString("OK");
	
	
}
bool EC25_SSL::connect(String ip,int port,uint8_t mode)
{
	
/*	AT+QSSLOPEN=1,1,4,"notify-api.line.me",443,0
	AT+QSSLOPEN=1,1,4,"https://notify-api.line.me/api/notify",443,0
	AT+QSSLOPEN=1,1,4,"203.104.138.174",443,0
	
	AT+QSSLOPEN=1,1,4,"172.217.174.163",443,0
	203.104.138.174
	*/
	
	_connected = 0;
	bool ret = false;
	//AT+QSSLOPEN=1,1,4,"220.180.239.212",8010,0
	LTE.ECser.sendAT("AT+QSSLOPEN=",false);
	LTE.ECser.sendAT(String(_pdpctxID),",",false);
	LTE.ECser.sendAT(String(_sslctxID),",",false);
	LTE.ECser.sendAT(String(_clientID),",",false);
	
	LTE.ECser.sendAT("\"",false);
	LTE.ECser.sendAT(ip,false);
	LTE.ECser.sendAT("\"",",",false);
	LTE.ECser.sendAT(String(port),false);
	LTE.ECser.sendAT(",",String(mode));
	
	EC_Resp res = LTE.ECser.waitString(EC25_RET_STR,RET_STR_NUM,10000);
	//checkURC(res.id,res.data);
	if(res.id==0)
	{	
		res = LTE.ECser.waitString(EC25_RET_STR,RET_STR_NUM,10000);
		//checkURC(res.id,res.data);
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
				//getLastError();
			}				
		}
	}
	else if (res.id==3)
	{
		ret = true;
	}
	else
	{	//getLastError();	
	}
	_connectMode = mode;
	return ret;
	
}
bool EC25_SSL::startSend()
{
	String str = "AT+QSSLSEND="+String(_clientID);
	LTE.ECser.sendAT(str);
	EC_Resp res = LTE.ECser.waitString(EC25_RET_STR,RET_STR_NUM,3000);
	//checkURC(res.id,res.data);
	//Serial.println(res.data);
	if(res.id==4)
		return true;
	else
	{	
		/*int err = getLastError();
		if(err == 572)
		{
			_connected = 0;
		}
		*/
	}
	return false;
}
bool EC25_SSL::stopSend()
{
	LTE.ECser.write(0x1A);
	EC_Resp res = LTE.ECser.waitString(EC25_RET_STR,RET_STR_NUM,3000);
	if(res.id==5||res.id==0) // OK or SEND OK
		return true;
	return false;
}
size_t EC25_SSL::readBytes(uint8_t *data,size_t length)
{
    //AT+QSSLRECV=4,1500
	//+QSSLRECV: 374
	EC_Resp res; 
	//res = LTE.ECser.waitString("+QSSLURC:",5000);
	int len =0;
	String str = "AT+QSSLRECV="+String(_clientID)+","+String(length);
	LTE.ECser.sendAT(str);
	String resp[2]={"+QSSLRECV:","ERROR"};
	res = LTE.ECser.waitString(resp,2,5000);
	//Serial.println(res.data);
	if(res.id==0)
	{
		int index  = res.data.lastIndexOf(" ");
		len = res.data.substring(index+1).toInt();
		LTE.ECser.readBytes(data,len);
	}
	res = LTE.ECser.waitString("OK");
	res = LTE.ECser.waitString("+QSSLURC:",2000);
	return len;	
}
bool EC25_SSL::waitData(long tout)
{
	EC_Resp res; 
	res = LTE.ECser.waitString("+QSSLURC:",tout);
	if(res.id==0)
		return true;
	return false;
}
bool EC25_SSL::close()
{
	String str = "AT+QSSLCLOSE="+String(_clientID);
	LTE.ECser.sendAT(str);
	EC_Resp res = LTE.ECser.waitString("OK",2000);
	if(res.id==0)
		return true;
	return false;
}


