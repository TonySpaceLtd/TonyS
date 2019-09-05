
#ifndef TonySpace_NBIoT_TCP_h
#define TonySpace_NBIoT_TCP_h

#include "Arduino.h"
#include <Stream.h>
#include "TonyS_X1.h"
#include "TonySpace_NBIoT.h"

#define REQ_LENGTH 1358

class NBIoT_TCP : public Stream 
{
	public:
		NBIoT_TCP()
		{ 
			socket_ = 0xFF;
			head = 0;
			tail = 0;
		};
		uint8_t createSocket(int port,bool receiveCtrl = false,bool ipv6= false);
		bool connect(String ip,int port);
		
		bool write(uint8_t *data,size_t length);
		bool print(String data);
		bool println(String data){return print(data+"\r\n");};
		
		size_t printf(const char *format, ...);
		
		size_t tcpAvailable(unsigned long tout=5000);
		int strToHex(char *hstr);
		bool connected();
		int tcpRead();
		bool closeSocket();
		
		
		//Stream Class 
		virtual int read(){return tcpRead();};	
		virtual size_t write(uint8_t c){};
		virtual int available(){return tcpAvailable();};
		virtual int peek(){}; 
		virtual void flush(){};
		using Stream::write; // make the other overloads of write visible
	
	private:
		uint8_t socket_;
		char *reciveBuf;
		size_t head;
		size_t tail;
		
};
#endif





















