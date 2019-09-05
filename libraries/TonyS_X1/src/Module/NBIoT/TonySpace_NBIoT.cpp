#include "TonySpace_NBIoT.h"
#include "TonyS_X1.h"

NBIoT_SER nbSer;

NBIoT::NBIoT()
{
	
}
void NBIoT::begin(uint8_t slot,unsigned long baud)
{
	nbSer.begin(slot,baud);
	
	if(slot>=SLOT1&&slot<=SLOT6_U)
	{
		nbPowerPin = getPinIO[slot];
		delay(10);
		Tony.pinMode(nbPowerPin, OUTPUT);
		delay(10);
		Tony.digitalWrite(nbPowerPin, LOW);
		powerOn(true);
		setConfig();
	}
}
void NBIoT::powerOn(bool on_off)
{
	Tony.digitalWrite(nbPowerPin, on_off);
}
bool NBIoT::powerOnReady(long tout)
{
	
	if(nbSer.waitString("Neul",tout).id==0)
	{
		if(nbSer.waitString("OK",tout/2).id==0)
			return true;
	}
	//Serial.println("Power On TimeOut");
	return false;
}
void NBIoT::setConfig()
{
	while(powerOnReady(10000)){}
	nbSer.sendAT(F("ATE0"));
	nbSer.waitString(F("OK"));
	nbSer.sendAT(F("AT+QLEDMODE=1"));
	nbSer.waitString(F("OK"));
	nbSer.sendAT(F("AT+QREGSWT=2"));
	nbSer.waitString(F("OK"));
	nbSer.sendAT(F("AT+QIDNSCFG=8.8.8.8,8.8.4.4"));
	nbSer.waitString(F("OK"));
	if(!setAutoConnect())
	{
		Serial.println("REBOOTING");
		nbSer.sendAT(F("AT+NRB"));
		nbSer.waitString(F("REBOOTING"));
		while(powerOnReady(10000)){}
		Serial.println("Ready..");
	}
}
bool NBIoT::setAutoConnect()
{
	nbSer.sendAT(F("AT+NCONFIG?"));
	atResp res = nbSer.waitString(F("AUTOCONNECT"),3000);
	if(res.id==0)
	{
		//Serial.println(res.data);
		if(res.data.indexOf("FALSE")!=-1)
		{
			nbSer.waitString(F("OK"),3000);
			Serial.println("Set Auto Connect");
			nbSer.sendAT(F("AT+NCONFIG=AUTOCONNECT,TRUE"));
			nbSer.waitString(F("OK"),3000);
			return false;
		}
	}
	nbSer.waitString(F("OK"),3000);
	return true;
}
String NBIoT::getIMEI()
{
	String str_imei="";
	nbSer.sendAT(F("AT+CGSN=1"));
	atResp res = nbSer.waitString(F("+CGSN:"));
	if(res.id==0)
	{
		int index = res.data.indexOf(":");
		str_imei = res.data.substring(index+1);
	}
	nbSer.waitString(F("OK"),3000);
	return(str_imei);
}
String NBIoT::getSIMSerial()
{
	String str ="";
	nbSer.sendAT(F("AT+NCCID"));
	atResp res = nbSer.waitString(F("+NCCID"));
	if(res.id==0)
	{
		int index = res.data.indexOf(":");
		str = res.data.substring(index+1);
	}
	nbSer.waitString(F("OK"),3000);
	return(str);
}
String NBIoT::getIMSI()
{
	String str ="";
	nbSer.sendAT(F("AT+CIMI"));
	atResp res = nbSer.waitString(F("OK"));
	if(res.id==0)
	{
		uint8_t len = res.temp.length();
		for(uint8_t i=0;i<len;i++)
		{
			if(res.temp[i]>=0x30 && res.temp[i]<=0x39)
				str += res.temp[i];
		}	
	}
	return(str);
}

bool NBIoT::getNetworkStatus()
{
	bool ret = false;
	nbSer.sendAT(F("AT+CEREG?"));
	atResp res = nbSer.waitString(F("+CEREG:0,1"),3000);
	if(res.id==0)
	{
		ret = true;
	}
	nbSer.waitString(F("OK"));
	return ret;
}
uint8_t NBIoT::getSignal()
{
	uint8_t ret=0;
	nbSer.sendAT(F("AT+CSQ"));
	atResp res = nbSer.waitString(F("+CSQ:"));
	if(res.id==0)
	{
		int index = res.data.indexOf(":");
		ret = res.data.substring(index+1).toInt();
	}
	nbSer.waitString(F("OK"));
	return ret;
}
int NBIoT::getSignaldBm()
{
	int ret = 255;
	uint8_t sign = getSignal();
	if(sign!=99)
		ret = map(sign,0,31,-113,-53);
	return ret;
}
uint8_t NBIoT::getSignalLevel()
{
	uint8_t ret = 255;
	uint8_t sign = getSignal();
	if(sign!=99)
		ret = map(sign,0,31,0,5);
	return ret;
}
String NBIoT::getDeviceIP()
{
	String str ="";
	nbSer.sendAT(F("AT+CGPADDR"));
	atResp res = nbSer.waitString(F("+CGPADDR"));
	if(res.id==0)
	{
		uint8_t len = res.temp.length();
		int index = res.data.indexOf(",");
		str = res.data.substring(index+1);
	}
	nbSer.waitString(F("OK"));
	return(str);
}
























