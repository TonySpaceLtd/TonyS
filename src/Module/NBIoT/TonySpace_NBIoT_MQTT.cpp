#include "TonySpace_NBIoT_MQTT.h"

void func_null(String topic ,char *payload,size_t length){}
NBIoT_MQTT::NBIoT_MQTT()
{
		rxCnt = 0;	
		callback = func_null;
		status =false;
}	
bool NBIoT_MQTT:: open(String serverIP,int port)
{
	ser_nbiot.flush();
	ser_nbiot.flush();
	ser_nbiot.printf("AT+QMTOPEN=0,\"");
	ser_nbiot.print(serverIP);
	ser_nbiot.printf("\",%d\r\n",port);
	
	atResp res = nbSer.waitString(F("+QMTOPEN"),3000);
	if(res.id==0)
	{
		if(res.data.indexOf("0,0")!=-1)
		{
			if(psramFound())
			{
				reciveBuf = (char*)heap_caps_malloc(REQ_LENGTH,MALLOC_CAP_8BIT);
			}
			else
				reciveBuf = (char*)malloc(REQ_LENGTH);
			if (reciveBuf == NULL) 
			{
				Serial.println("Failed to allocate memory");
				return ESP_ERR_NO_MEM;
			}
			return true;
		}
	}
	return false;
}
bool NBIoT_MQTT:: connect(String clientID,String user,String password)
{
	//QMTCONN=0,"clientExample","qmzdfdtb","owAXNI-sLvOk"
	ser_nbiot.print("AT+QMTCONN=0,");
	ser_nbiot.print("\"");
	ser_nbiot.print(clientID);
	ser_nbiot.print("\"");
	if(user.length()>0)
	{
		ser_nbiot.print(",\"");
		ser_nbiot.print(user);
		ser_nbiot.print("\"");
	}
	if(password.length()>0)
	{
		ser_nbiot.print(",\"");
		ser_nbiot.print(password);
		ser_nbiot.print("\"");
	}
	ser_nbiot.println();
	atResp res = nbSer.waitString(F("+QMTCONN"),10000);
	if(res.id==0)
	{
		if(res.data.indexOf("0,0,0")!=-1)
		{
			status =true;
			return true;
		}
		else
		{
			Serial.print("Error");
			Serial.println(res.data);
		}
	}
	return false;
}
bool NBIoT_MQTT:: subScribe(String topic,uint8_t qos)
{
	bool ret=false;
	static int  msgid=1;
	ser_nbiot.printf("AT+QMTSUB=0,%d,\"",msgid);
	ser_nbiot.print(topic+"\",");
	ser_nbiot.println(qos);
	atResp res = nbSer.waitString(F("+QMTSUB"),15000);
	if(res.id==0)
	{
		String str = "0,"+String(msgid)+",0,0";
		if(res.data.indexOf(str)!=-1)
			ret= true;
		else
			Serial.println(res.data);
	}
	msgid++;
	if(msgid==0)
		msgid=1;
	
	return ret;
}
bool NBIoT_MQTT:: unSubScribe(String topic)
{
	bool ret=false;
	static int  msgid=1;
	ser_nbiot.printf("AT+QMTUNS=0,%d,\"",msgid);
	ser_nbiot.print(topic);
	ser_nbiot.println("\"");
	atResp res = nbSer.waitString(F("+QMTUNS"),15000);
	if(res.id==0)
	{
		String str = "0,"+String(msgid)+",0";
		if(res.data.indexOf(str)!=-1)
			ret= true;
		else
			Serial.println(res.data);
	}
	msgid++;
	if(msgid==0)
		msgid=1;
	return ret;
}
bool NBIoT_MQTT:: publish(String topic,uint8_t *data,size_t length,uint8_t qos,uint8_t retain)
{
	eventTask();
	
	bool ret=false;
	static int  msgid=0;
	msgid++;
	if(qos==0)
		msgid=0;
	else
		msgid++;
	ser_nbiot.printf("AT+QMTPUB=0,%d,%d,%d,\"",msgid,qos,retain);
	
	ser_nbiot.println(topic+"\"");
	atResp res = nbSer.waitString(F(">"));
	if(res.id==0)
	{
			ser_nbiot.write(data,length);
			ser_nbiot.write(0x1A);
	}
	res = nbSer.waitString(F("+QMTPUB"),5000);
	if(res.id==0)
	{
		String str = "0,"+String(msgid)+",0";
		if(res.data.indexOf(str)!=-1)
			ret= true;
		else
			Serial.println(res.data);
	}
	return ret;
}
bool NBIoT_MQTT:: publish(String topic,String data,uint8_t qos,uint8_t retain)
{
	return publish(topic,(uint8_t*)data.c_str(),data.length(),qos,retain);
}
void NBIoT_MQTT:: eventTask()
{
	while(ser_nbiot.available())
	{
		String data = ser_nbiot.readStringUntil(':');
		if(data.indexOf("+QMTRECV")!=-1)
		{
			qmtrecv();
		}
		if(data.indexOf("+QMTSTAT")!=-1)
		{
			qmtstat();
		}
	}
}
void NBIoT_MQTT:: qmtrecv()
{
	ser_nbiot.readStringUntil('\"');
	String topic = ser_nbiot.readStringUntil('\"');
	//Serial.println("Topic : " +topic);
	ser_nbiot.readStringUntil(',');
	delay(5);
	//Serial.print("Data : ");
	rxCnt=0;
	while(ser_nbiot.available())
	{
		for(uint8_t i=0;i<15;i++)
		{
			while(ser_nbiot.available())
			{
				//Serial.write(ser_nbiot.read());
				reciveBuf[rxCnt++] = ser_nbiot.read();
			}
			delay(10);
		}
		delay(100);
	}
	reciveBuf[rxCnt-2] = 0;
	(*callback)(topic,reciveBuf,rxCnt-2);
	//Serial.println("\r\n..End..");
}
void NBIoT_MQTT:: qmtstat()
{
	ser_nbiot.readStringUntil('\r');
	status =false;
	Serial.println("\r\n-->Connection Closed");
}
bool NBIoT_MQTT:: connected()
{
	return status;
}













