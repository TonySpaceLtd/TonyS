#ifndef TonySpace_NBIoT_UDP_h
#define TonySpace_NBIoT_UDP_h

#include "Arduino.h"
#include <Stream.h>
#include "TonyS_X1.h"
#include "TonySpace_NBIoT.h"

#define REQ_LENGTH 1358

class NBIoT_UDP : public Stream 
{
	public:
	NBIoT_UDP()
		{ 
			socket_ = 0xFF;
			head = 0;
			tail = 0;
			serverIP_="";
			port_ = 0;
		};
	uint8_t createSocket(String serverIP,int port,bool receiveCtrl = false,bool ipv6= false);
	void setServer(String serverIP,int port);
	bool write(uint8_t *data,size_t length);
	bool print(String data);
	bool println(String data){ return print(data);};
	size_t printf(const char *format, ...);
	size_t udpAvailable(unsigned long tout=5000);
	int strToHex(char *hstr);
	//bool connected();
	int udpRead();
	bool closeSocket();
	
	//Stream Class 
		virtual int read(){return udpRead();};	
		virtual size_t write(uint8_t c){};
		virtual int available(){return udpAvailable();};
		virtual int peek(){}; 
		virtual void flush(){};
		using Stream::write; // make the other overloads of write visible

	private:
		uint8_t socket_;
		char *reciveBuf;
		size_t head;
		size_t tail;
		String serverIP_;
		int port_;
};








































#endif