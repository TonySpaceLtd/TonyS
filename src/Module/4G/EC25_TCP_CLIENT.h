#ifndef ECXX_TCP_CLIENT_H
#define ECXX_TCP_CLIENT_H


#include "Arduino.h"
#include <Stream.h>
#include "TonyS_X1.h"
#include"EC25_lib.h"

#define BLYNK_CONNECT_ID 11 

class EC25Client : public Client
{
	public:
	EC25_TCP *blynkSocket;
	
	EC25Client(){
		
	};
	bool init()
	{
		blynkSocket =  new EC25_TCP (BLYNK_CONNECT_ID);
	}
	virtual int connect(const char *host, uint16_t port) 
	{
		blynkSocket->close();
		return blynkSocket->connect(host,port,CON_MODE_BUFFER);
	}
	 virtual int connect(IPAddress ip, uint16_t port)
	 {
		 return 1;
	 }
	virtual void stop() 
	{
		//Serial.println("Close");
		blynkSocket->close();
	}
	virtual size_t write(const uint8_t *buf, size_t size) 
	{
		return blynkSocket->writeBytes(buf,size);
	}
	virtual size_t write(uint8_t c) 
	{
		return write(&c, 1);
	}
	virtual int available() 
	{
		int len = blynkSocket->available();
		//if(len>0)
		//Serial.println("Data Len :"+String(len));
		return len;
	}
	virtual int read(uint8_t *buf, size_t size) 
	{
		 return blynkSocket->readBytes(buf,size);
	}
	virtual int read() 
	{
		uint8_t c;
		if (read(&c, 1) == 1) {
			return c;
		}
		return -1;
	}
	virtual int peek() { return -1; } //TODO
	virtual void flush() { }//blynkSocket->flush(); }

  virtual uint8_t connected() {
	  return blynkSocket->connected();
  }
  virtual operator bool() 
  { return 1; }


	
	
};























#endif