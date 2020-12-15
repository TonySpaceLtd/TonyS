#ifndef ECXX_TCP_H
#define ECXX_TCP_H

#include "Arduino.h"
#include <Stream.h>
#include "TonyS_X1.h"
#include"EC25_lib.h"

#define CON_MODE_BUFFER 		0 
#define CON_MODE_PUSH 			1
#define CON_MODE_TRANSPARENT 	2 

class EC25_TCP 
{
	public:
	EC25_TCP(uint8_t connectID=0);
	bool connect(String ip,int port,uint8_t mode=CON_MODE_BUFFER);
	bool startSend();
	bool stopSend();
	int  writeBytes(const uint8_t *buf, size_t size);
	int print(String str);
	int println(String str){ print(str+"\r\n");  };
	int available();
	int  read();
	int  readBytes(uint8_t *buffer,size_t len);
	void checkURC(uint8_t n,String data);
	uint8_t connected();
	int getLastError();
	bool close();
	void flush();
	void setTimeout(unsigned long t){};
	
	private:
	uint8_t _contextID;
	uint8_t _connectID;
	uint8_t _connectMode;
	uint8_t _connected;
};































#endif