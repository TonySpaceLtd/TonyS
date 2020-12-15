#include "EC25_SER.h"


HardwareSerial ser_LTE (NULL);

EC25_SER::EC25_SER()
{

}
void EC25_SER::begin(uint8_t slot,unsigned long baud)
{
	ser_LTE = Tony.SerialBegin(slot,baud);
}
EC_Resp EC25_SER::waitString(String data[],uint8_t len,long tout)
{
	EC_Resp res;
	unsigned long pv_t = 0;
	pv_t = millis();
	do
	{
		if(ser_LTE.available())
		{
			String serDat = ser_LTE.readStringUntil('\r'); 
			res.temp += serDat;
			for(uint8_t i=0;i<len;i++)
			{
				if(serDat.indexOf(data[i]) != -1)
				{
					res.id = i;
					res.data = serDat;
					return(res);
				}
			}
		}
	}
	while(millis()-pv_t<tout);
	res.id = -1;
	
	return(res);
}
EC_Resp EC25_SER::waitString(String data,long tout)
{
	String dataArr[1];
	dataArr[0] = data;
	return waitString(dataArr,1,tout);
}
void EC25_SER::sendAT(String data,bool end)
{
	ser_LTE.print(data);
	if(end)
		ser_LTE.println();
}
void EC25_SER::sendAT(String data,String data2,bool end)
{
	ser_LTE.print(data);
	ser_LTE.print(data2);
	if(end)
		ser_LTE.println();
}

String EC25_SER::removeChar(String str,char cut)
{
	String buf="";
	for(uint8_t i=0;i<str.length();i++)
	{
		if(str[i]!=cut)
		{
			char c = str[i];
			buf += c;
		}
	}
	return buf;
}
