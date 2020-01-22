#include "TonySpace_NBIoT_TCP.h"
#include "TonyS_X1.h"

uint8_t NBIoT_TCP::createSocket(int port,bool receiveCtrl,bool ipv6)
{
	ser_nbiot.flush();
	ser_nbiot.flush();
	ser_nbiot.printf("AT+NSOCR=STREAM,6,%d,%d",port,receiveCtrl);
	if(ipv6)
		ser_nbiot.print(F("AF_INET6"));
	ser_nbiot.println();
	atResp res = nbSer.waitString(F("OK"),3000);
	String str="";
	if(res.id==0)
	{
		uint8_t len = res.temp.length();
		for(uint8_t i=0;i<len;i++)
		{
			if(res.temp[i]>=0x30 && res.temp[i]<=0x39)
				str += res.temp[i];
		}	
		socket_ = str.toInt();
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
	
		
	}
	return socket_;
}
bool NBIoT_TCP::connect(String ip,int port)
{
	if(socket_==0xFF)
		return false;
	ser_nbiot.printf("AT+NSOCO=%d,",socket_);
	ser_nbiot.print(ip);
	ser_nbiot.printf(",%d\r\n",port);
	atResp res = nbSer.waitString(F("OK"),7000);
	if(res.id==0)
		return true;
	return false;
}
bool NBIoT_TCP::write(uint8_t *data,size_t length)
{
	if(socket_==0xFF)
		return false;
	ser_nbiot.printf("AT+NSOSD=%d,%d,",socket_,length);
	for(int i=0;i<length;i++)
	{
		uint8_t c = data[i];
		if(c<=16)	
			ser_nbiot.print("0");
		ser_nbiot.print(c,HEX);
	}
	ser_nbiot.println();
	atResp res = nbSer.waitString(F("OK"),7000);
	if(res.id==0)
		return true;
	return false;
}
bool NBIoT_TCP::print(String data)
{
	int length = data.length();
	
	return write((uint8_t*)data.c_str(),length);
}
size_t NBIoT_TCP::tcpAvailable(unsigned long tout)
{
	while(ser_nbiot.available())
	{
		String data = ser_nbiot.readStringUntil('\r');
		//Serial.println(data);
	}
	

	if(tail==0)
	{
		ser_nbiot.printf("AT+NSORF=%d,%d\r\n",socket_,REQ_LENGTH);
	}
	else
	{
		size_t len = tail - head;
		if(len==0)
				tail =0;
		return (len);
	}
	unsigned long pv_t = millis();

	//1,178.128.80.51,1234,22,48656C6C6F204E42496F542053657474616B616E0D0A,0
    //OK

	do
	{
		while(ser_nbiot.available())
		{
			String data = ser_nbiot.readStringUntil('\n');
			data = ser_nbiot.readStringUntil(',');
			if(data.indexOf(F("OK"))!=-1)
				return 0;
			if(data.indexOf(F("ERROR"))!=-1)
			{
				socket_ = 0xFF;
				return 0xFF;
			}
		//	Serial.println("Socket : "+ data);
			data = ser_nbiot.readStringUntil(',');
		//	Serial.println("IP : "+ data);
			
			data = ser_nbiot.readStringUntil(',');
		//	Serial.println("PORT : "+ data);
			
			data = ser_nbiot.readStringUntil(',');
		//	Serial.println("Length : "+ data);
		//	Serial.print("Data :");
			int length = data.toInt();
			tail = 0;
			head = 0;
			for(int i=0;i<length;i++)
			{
				char hexStr[2];
				ser_nbiot.readBytes(hexStr,2);
				reciveBuf[tail] = strToHex(hexStr);
		//		Serial.write(reciveBuf[tail]);
				tail++;
			}
		//	Serial.println();
			//data = ser_nbiot.readStringUntil(',');
			//Serial.println("Data : "+ data);
			data = ser_nbiot.readStringUntil(',');
			data = ser_nbiot.readStringUntil('\r');
		//	Serial.println("Next Length : "+ data);
			
		//	atResp res = nbSer.waitString(F("OK"),6000);
		    ser_nbiot.readStringUntil('\r');
			ser_nbiot.readStringUntil('\n');
			//Serial.println("Tail "+String(tail));
			return tail;
		}
	}
	while((millis()-pv_t)<tout);
	Serial.println("Time Out");
	return 0;
}

int NBIoT_TCP::strToHex(char *hstr)
{
	return (int) strtol(hstr, 0, 16);
}
bool NBIoT_TCP::connected()
{
	if(socket_==0xFF)
		return false;
	return true;
}
int NBIoT_TCP::tcpRead()
{
	if(head < tail)
		return reciveBuf[head++];
	else
	{
		tail = 0;
		return -1;
	}
}
bool NBIoT_TCP::closeSocket()
{
	ser_nbiot.printf("AT+NSOCL=%d",socket_);
	atResp res = nbSer.waitString(F("OK"),7000);
	if(res.id==0)
		return true;
	return false;
}

size_t NBIoT_TCP::printf(const char *format, ...)
{
    char loc_buf[64];
    char * temp = loc_buf;
    va_list arg;
    va_list copy;
    va_start(arg, format);
    va_copy(copy, arg);
    size_t len = vsnprintf(NULL, 0, format, copy);
    va_end(copy);
    if(len >= sizeof(loc_buf)){
        temp = new char[len+1];
        if(temp == NULL) {
            return 0;
        }
    }
    len = vsnprintf(temp, len+1, format, arg);
    
	write((uint8_t*)temp, len);
    
	va_end(arg);
    if(len >= sizeof(loc_buf)){
        delete[] temp;
    }
    return len;
}













