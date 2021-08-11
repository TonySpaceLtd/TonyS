#include "EC25_MQTT.h"

void func_null_(String topic ,uint8_t *payload,size_t length){}

EC25_MQTT::EC25_MQTT()
{
	_connected=0;
	callback = func_null_;
}
void EC25_MQTT::init(uint8_t client_idx)
{
	_client_idx=client_idx;
	EC_Resp res;
	String str = "AT+QMTCFG=\"recv/mode\","+String(_client_idx)+",1,0";
	LTE.ECser.sendAT(str);
	res = LTE.ECser.waitString("OK");
	
}
bool EC25_MQTT::connect(String ip,int port,String deviceID,String user,String password)
{
	bool ret=false;
	//AT+QMTOPEN=0,"m16.cloudmqtt.com",18568
	String str = "AT+QMTOPEN=" + String(_client_idx) + ",\"" + ip +"\"," + String(port);
	EC_Resp res;
	LTE.ECser.sendAT(str);
	res = LTE.ECser.waitString("OK");
	res = LTE.ECser.waitString("+QMTOPEN:",10000);
	if(res.id==0)
	{
		//+QMTOPEN: 5,0
		str = "+QMTOPEN: "+String(_client_idx)+",0";
		if(res.data.indexOf(str)!=-1)
		{
			//Serial.println("Connect to Server: OK");
			str = "AT+QMTCONN="+String(_client_idx) + ",\"" + deviceID + "\"";
			if(user.length()>0)
				str += ",\""+user+"\"";
			if(password.length()>0)
				str += ",\""+password+"\"";
			LTE.ECser.sendAT(str);
			res = LTE.ECser.waitString("OK");
			res = LTE.ECser.waitString("+QMTCONN:",5000);
			if(res.id==0)
			{
				int index  = res.data.lastIndexOf(",");
				int status = res.data.substring(index+1).toInt();
				switch(status)
				{
					case 0:
						Serial.println("Connection Accepted");
						ret =true;
						_connected=1;
					break;
					case 1:
						Serial.println("Connection Refused: Unacceptable Protocol Version");
					break;
					case 2:
						Serial.println("Connection Refused: Identifier Rejected");
					break;
					case 3:
						Serial.println("Connection Refused: Server Unavailable");
					break;
					case 4:
						Serial.println("Connection Refused: Bad User Name or Password");
					break;
					case 5:
						Serial.println("Connection Refused: Not Authorized");
					break;
				}
				if(status!=0)
					res = LTE.ECser.waitString("+QMTSTAT:",5000);
			}
			
		}
	}
	return ret;
}
bool EC25_MQTT::disConnect()
{
	String str = "AT+QMTDISC=" + String(_client_idx);
	EC_Resp res;
	LTE.ECser.sendAT(str);
	res = LTE.ECser.waitString("OK");
	res = LTE.ECser.waitString("+QMTDISC",5000);
	if(res.id==0)
	{
		int index  = res.data.lastIndexOf(",");
		int status = res.data.substring(index+1).toInt();
		if(status==0)
			return true;
		
	}
	return false;	
}
bool EC25_MQTT::close()
{
	String str = "AT+QMTCLOSE=" + String(_client_idx);
	EC_Resp res;
	LTE.ECser.sendAT(str);
	res = LTE.ECser.waitString("OK");
	res = LTE.ECser.waitString("+QMTCLOSE",5000);
	if(res.id==0)
	{
		int index  = res.data.lastIndexOf(",");
		int status = res.data.substring(index+1).toInt();
		if(status==0)
			return true;
	}
	return false;	
}
bool EC25_MQTT::subScribe(String topic[],uint8_t qos[],uint8_t size)
{
	bool   ret=false;
	String str1 = "AT+QMTSUB="+String(_client_idx)+",1";
	String str2="";
	for(uint8_t i=0;i<size;i++)
	{
		str2 += ",";
		str2 += "\"";
		str2 += topic[i];
		str2 += "\",";
		str2 += String(qos[i]);
	}
	
	EC_Resp res;
	LTE.ECser.sendAT(str1+str2);
	res = LTE.ECser.waitString("OK");
	res = LTE.ECser.waitString("+QMTSUB",5000);
	if(res.id==0)
	{
		int index   = res.data.indexOf(",");
		int index2  = res.data.indexOf(",",index+1);
		int index3  = res.data.indexOf(",",index2+1);
		int status = res.data.substring(index2+1,index3).toInt();
		if(status==0)
			ret=true;
	}
	return ret;
}
bool EC25_MQTT::subScribe(String topic[],uint8_t qos,uint8_t size)
{
	 uint8_t qos_[size];
	for(uint8_t i =0;i<size;i++)
		qos_[i] = qos;
	return subScribe(topic,qos_,size);
}
bool EC25_MQTT::subScribe(String topic,uint8_t qos)
{
	String topic_[1];
    uint8_t qos_[1]; 
	topic_[0] 	= topic;
    qos_[0]		= qos;
   return subScribe(topic_,qos_,1);
}
bool EC25_MQTT::unSubScribe(String topic[],uint8_t size)
{
	bool ret=false;
	String str1 = "AT+QMTUNS="+String(_client_idx)+",1";
	String str2="";
	for(uint8_t i=0;i<size;i++)
	{
		str2 += ",";
		str2 += "\"";
		str2 += topic[i];
		str2 += "\"";
	}
	//Serial.println("\r\n"+str1+str2);
	EC_Resp res;
	LTE.ECser.sendAT(str1+str2);
	res = LTE.ECser.waitString("OK");
	res = LTE.ECser.waitString("+QMTUNS",5000);
	if(res.id==0)
	{
		int index   = res.data.indexOf(",");
		int index2  = res.data.indexOf(",",index+1);
		int index3  = res.data.indexOf(",",index2+1);
		int status = res.data.substring(index2+1,index3).toInt();
		if(status==0)
			ret=true;
	}
	return ret;
}
bool EC25_MQTT::unSubScribe(String topic)
{
	String topic_[1]={topic};
	return unSubScribe(topic_,1);
}
void EC25_MQTT::task()
{
	//AT+QMTRECV?
	
	String str;
	EC_Resp res;
	
	str = "AT+QMTRECV?";
	LTE.ECser.sendAT(str);
	
	str = "+QMTRECV: "+String(_client_idx)+",0,0,0,0,0";
	String str2[3]={"OK",str,"ERROR"};
	res = LTE.ECser.waitString(str2,3);
	if(res.id==0)
	{
		//_connected=0;
		//return;
	}
	if(res.id==1)
	{
		res = LTE.ECser.waitString("OK");
		return;
	}
	if(res.id==2)
	{
		res = LTE.ECser.waitString("OK");
		_connected=0;
		return;
	}
	res = LTE.ECser.waitString("OK");

	for(uint8_t i=0;i<5;i++)
	{
		str = "AT+QMTRECV="+String(_client_idx)+","+String(i);
		LTE.ECser.sendAT(str);
		String resp[3]={"+QMTRECV:","OK","ERROR"};
		res = LTE.ECser.waitString(resp,3,2000);
		if(res.id==0)
		{
			//+QMTRECV: 5,0,"LED1","dsfdsfdsf"

			int index = res.data.indexOf(",");
			int index2 = res.data.indexOf(",",index+1);
			if(index2==-1)
				i=i-1;
			else
			{
				//Serial.println(res.data);
				int index3 = res.data.indexOf(",",index2+1);
				int index4 = res.data.indexOf("\"",index3+2);
				String topic = res.data.substring(index2+2,index3-1);
				String payload = res.data.substring(index3+2,index4);
				size_t len = payload.length()+1;
				uint8_t buf[len];
				payload.toCharArray((char*)buf,len);
				
				//Serial.print("Topic:");Serial.println(topic);
				//Serial.print("Payload:");Serial.println(payload);
				(*callback)(topic,buf,len);
			}			
			//int status = res.data.substring(index+1,index2);
		}
		else if(res.id==1)
		{
			//Empty data case--> to do..
		}
		else if(res.id==2)
		{
			_connected=0;
			return;
		}
		else
		{
			Serial.print("Unknow Resp:");
			Serial.println(res.temp);
		}
	}
	
}
uint8_t EC25_MQTT::connected()
{
	for(uint8_t i=0;i<3;i++)
	{
		LTE.ECser.flush();
		LTE.ECser.sendAT(F("AT+QMTCONN?\r"));
		EC_Resp res;
		String str2[3]={"OK","+QMTCONN:","ERROR"};
		res = LTE.ECser.waitString(str2,3,1000);
		if(res.id==1)
		{
			int index1 = res.data.indexOf(":");
			int index2 = res.data.indexOf(",");
			int id = res.data.substring(index1+1,index2).toInt();
			int state = res.data.substring(index2+1).toInt();
			if(id == _client_idx)
			{
				if(state == 4)
					return false;
				else
					return true;
					
			}
		}
	}
	
	return false;
}
bool EC25_MQTT::publish(String topic,uint8_t *payload,size_t msglen,uint8_t msgID,uint8_t qos,uint8_t retain)
{
	String str;
	EC_Resp res;
	str = "AT+QMTPUBEX="+String(_client_idx)+","+String(msgID)+","+String(qos)+","+String(retain);
	str += ","+String("\"")+topic+String("\"")+","+String(msglen);
	LTE.ECser.sendAT(str);
	res = LTE.ECser.waitString(">",2000);
	if(res.id==0)
	{
		LTE.ECser.write(payload,msglen);
		String str_res2[2]={"OK","ERROR"};
		res = LTE.ECser.waitString(str_res2,2,5000);
		if(res.id==1)
		{
			return false;
		}
		res = LTE.ECser.waitString("+QMTPUBEX:",10000);	
		if(res.id==0)
		{
			int index = res.data.lastIndexOf(",");
			int status = res.data.substring(index+1).toInt();
			if(status==0)
				return true;
		}
	}
	return false;
	//Serial.println(str);
}
bool EC25_MQTT::publish(String topic,String payload,uint8_t msgID,uint8_t qos,uint8_t retain)
{
	return publish(topic,(uint8_t*)payload.c_str(),payload.length(),msgID,qos,retain);
}














