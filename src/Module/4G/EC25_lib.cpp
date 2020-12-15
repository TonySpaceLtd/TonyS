#include"EC25_lib.h"

EC25 LTE;

EC25::EC25()
{
	
}
void EC25::initial(uint8_t slot)
{
	if(slot>=SLOT1&&slot<=SLOT4_U)
	{
		powerPin = getPinIO[slot];
		Tony.pinMode(powerPin,OUTPUT);
		ECser.begin(slot,115200);
	}
}
void EC25::powerOn(bool onoff)
{
	Tony.digitalWrite(powerPin,onoff);
}
void EC25::reset()
{
	Tony.digitalWrite(powerPin,HIGH);
	delay(500);
	Tony.digitalWrite(powerPin,LOW);
}
bool EC25::waitModuleReady(uint8_t timeout)
{
	unsigned long last = millis();
	do
	{
		if(ECser.available())
		{
			String str = ECser.readStringUntil('\r'); 
			if(str.indexOf("RDY")!=-1)
			{
				setDefaultConfig();
				return true;
			}
		}
	}
	while((millis()-last)<((1000*60)*timeout));
	Serial.println("TimeOut: Module not Ready");
	return false;
}
void EC25::setDefaultConfig()
{
	ECser.sendAT(F("ATE0"));
	ECser.waitString(F("OK"));	
}
String EC25::getIMEI()
{
	String str_imei="ERROR";
	ECser.sendAT(F("AT+CGSN"));
	EC_Resp res = ECser.waitString(F("OK"));
	if(res.id==0)
	{
		String out = removeChar(res.temp,0x0A);
		res.temp = out;
		out = removeChar(res.temp,'O');
		res.temp = out;
		out = removeChar(res.temp,'K');
		str_imei = out;//res.temp;
	}
	return(str_imei);	
}
String EC25::getCIMI()
{
	String str_="ERROR";
	ECser.sendAT(F("AT+CIMI"));
	EC_Resp res = ECser.waitString(F("OK"));
	if(res.id==0)
	{
		String out = removeChar(res.temp,0x0A);
		res.temp = out;
		out = removeChar(res.temp,'O');
		res.temp = out;
		out = removeChar(res.temp,'K');
		str_ = out;//res.temp;
	}
	return(str_);
}

String EC25::getOperator_()
{
	String str_="ERROR";
	ECser.sendAT(F("AT+COPS?"));
	EC_Resp res = ECser.waitString(F("+COPS:"));
	if(res.id==0)
	{
		String out = removeChar(res.data,0x0A);
		str_ = out;
	}
	ECser.waitString(F("OK"));
	return(str_);
}
String EC25::getOperator()
{
	String str = getOperator_();
	int index  = str.indexOf("\"");
	int index2  = str.indexOf("\"",index+1);
	return str.substring(index+1,index2);
}
String EC25::getAccessTech()
{
	String str = getOperator_();
	int index  = str.lastIndexOf(",");
	int m =  str.substring(index+1).toInt();
	switch(m)
	{
		case 0:
			str = "GSM";
		break;
		case 2:
			str = "UTRAN";
		break;
		case 3:
			str = "GSM W/EGPRS";
		break;
		case 4:
			str = "UTRAN W/HSDPA";
		break;
		case 5:
			str = "UTRAN W/HSDPA and HSUPA";
		break;
		case 6:
			str = "UTRAN W/HSDPA and HSUPA";
		break;
		case 7:
			str = "E-UTRAN";
		break;
		case 100:
			str = "CDMA";
		break;
	}
	return(str);
}
int EC25::getSignaldBm()
{
	int sig = getSignal();
	if(sig>=99)
		return(0);
	if(sig==0)
		return -113;
	if(sig==1)
		return -111;
	if((sig>=2)&&(sig<=30))
		return map(sig,2,30,-109,-53);
	if(sig==31)
		return -51;
	
	
}
int EC25::getSignal()
{
	int signal=0;
	for(uint8_t i=0;i<10;i++)
	{
		signal = getSignal_();
		if(signal!=99)
		{
			break;
		}
		delay(500);
	}
	if(signal==99)
		signal=0;
	return signal;
}
int EC25::getSignal_()
{
	uint8_t ret=0;
	ECser.sendAT(F("AT+CSQ"));
	EC_Resp res = ECser.waitString(F("+CSQ:"));
	if(res.id==0)
	{
		int index = res.data.indexOf(" ");
		int index2 = res.data.indexOf(",");	
		ret = res.data.substring(index+1,index2).toInt();
	}
	ECser.waitString(F("OK"));
	return ret;
}
bool EC25::getNetworkRegis()
{
	bool ret = false;
	ECser.sendAT(F("AT+CREG?"));
	EC_Resp res = ECser.waitString(F("+CREG:"));
	
	if(res.id==0)
	{
		if(res.data.indexOf(",1")!=-1)
			ret = true;
	}
	ECser.waitString(F("OK"));
	return ret;
}


bool EC25::connectInternet()
{
	//AT+QICSGP=1,1,"INTERNET","","",1
	ECser.sendAT(F("AT+QICSGP=1,1,\"INTERNET\",\"\",\"\",1"));
	EC_Resp res = ECser.waitString(F("OK"));
	if(res.id==0)
	{
		ECser.sendAT(F("AT+QIACT=1"));
		res = ECser.waitString(F("OK"),150*1000);
		if(res.id==0)
			return true;
	}
	return false;
}
bool EC25::disconnectInternet()
{
	ECser.sendAT(F("AT+QIDEACT=1"));
	EC_Resp res = ECser.waitString(F("OK"),1000);
	if(res.id==0)
		return true;
	else
		return false;
}
String EC25::getIPAddress()
{
	String ip="";
	ECser.sendAT(F("AT+QIACT?"));
	EC_Resp res = ECser.waitString(F("+QIACT:"));
	if(res.id==0)
	{
		int index = res.data.indexOf("\"");
		int index2 = res.data.lastIndexOf("\"");
		ip =  res.data.substring(index+1,index2);
	}
	res = ECser.waitString(F("OK"),1000);
	return ip;
}




