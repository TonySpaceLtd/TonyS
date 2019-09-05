#include "TonySpace_NBIoT_SER.h"
#include "TonyS_X1.h"


HardwareSerial ser_nbiot(NULL);

NBIoT_SER::NBIoT_SER()
{
	
}
void NBIoT_SER::begin(uint8_t slot,unsigned long baud)
{
	ser_nbiot = Tony.SerialBegin(slot,baud);
}
atResp NBIoT_SER::waitString(String data[],uint8_t len,long tout)
{
	atResp res;
	unsigned long pv_t = 0;
	pv_t = millis();
	do
	{
		if(ser_nbiot.available())
		{
			String serDat = ser_nbiot.readStringUntil('\r'); 
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
atResp NBIoT_SER::waitString(String data,long tout)
{
	String dataArr[1];
	dataArr[0] = data;
	return waitString(dataArr,1,tout);
}
void NBIoT_SER::sendAT(String data,bool end)
{
	ser_nbiot.print(data);
	if(end)
		ser_nbiot.println();
}