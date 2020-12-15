#ifndef ECXX_SSL_H
#define ECXX_SSL_H

#include "Arduino.h"
#include <Stream.h>
#include "TonyS_X1.h"
#include "EC25_lib.h"

#define CON_MODE_BUFFER 		0 
#define CON_MODE_PUSH 			1
#define CON_MODE_TRANSPARENT 	2 

class EC25_SSL
{
	public:
		EC25_SSL(uint8_t connectID=0);
		void init();
		bool connect(String ip,int port,uint8_t mode);
		bool startSend();
		bool stopSend();
		size_t readBytes(uint8_t *data,size_t length);
		bool waitData(long tout);
		bool close();
	
	private:
		uint8_t _pdpctxID;  //1-16
		uint8_t _sslctxID;  //0-5
		uint8_t _clientID;  //0-11
		uint8_t _connected;
		uint8_t _connectMode;
	
};

































#endif