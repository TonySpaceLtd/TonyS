#include "EC25_GNSS.h"

EC25_GNSS::EC25_GNSS()
{
	
}
bool EC25_GNSS::open()
{
	EC_Resp res;
	LTE.ECser.sendAT("AT+QGPS=1");
	res = LTE.ECser.waitString("OK");
	if(res.id==0)
		return true;
	return false;
}
bool EC25_GNSS::close()
{
	EC_Resp res;
	LTE.ECser.sendAT("AT+QGPSEND");
	res = LTE.ECser.waitString("OK");
	if(res.id==0)
		return true;
	return false;
}
String EC25_GNSS::get(uint8_t mode)
{
	//516  not fix
	//505  not open
	EC_Resp res;
	LTE.ECser.sendAT("AT+QGPSLOC=",String(mode));
	String resp[2]={"+QGPSLOC:","+CME ERROR:"};
	res = LTE.ECser.waitString(resp,2,2000);
	if(res.id==0)
	{
		return 	LTE.ECser.removeChar(res.data,'\n');
	}
	if(res.id==1)
	{
		if(res.data.indexOf("516")!=-1)
			return "No Fix";
		if(res.data.indexOf("505")!=-1)
			return "GNSS not active";
	}
	return "Error";

}
GNSS_Resp EC25_GNSS::getData(uint8_t mode)
{
	GNSS_Resp ret;
	ret.status = false;
	String dat = get(mode);
	
	if(dat.indexOf("+QGPSLOC")!=-1)
	{
		Serial.println(dat);
		ret.status=true;
		int index =  dat.indexOf(",");
		int index2 = dat.indexOf(",",index+1);
		ret.latitude = dat.substring(index+1,index2);
		
		int index3 = dat.indexOf(",",index2+1);	
		ret.longitude = dat.substring(index2+1,index3);	
		ret.latitude_Float = ret.latitude.toFloat();
		ret.longitude_Float= ret.longitude.toFloat();		
	}
	return ret;
}
















