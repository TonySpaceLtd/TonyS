#ifndef UC20_UDP_H
#define UC20_UDP_H

#include <Arduino.h>
#include "UC20_lib.h"
#include "UC20_internet.h"

class UC20_UDP : public UC20_internet {
	public:
		UC20_UDP(int8_t conid, int8_t ctxid = 1);
		bool connect(const char* server, const char* port);
		bool connect(const char* server, unsigned int port);
		bool connected();
		bool disconnect();
		bool beginSend(int len = -1);	// if (len = -1), length will set as dynamic
		bool endSend();
		int8_t waitrecv(unsigned long t_out = 12000);
		long available();
		int read();
		String readString();
		
	protected:
		int8_t _connectid;		// range (0-11)
		
	private:
		int _userid;
};

#endif
