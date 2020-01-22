#ifndef UC20_INTERNET_H
#define UC20_INTERNET_H

#include <Arduino.h>
#include "UC20_lib.h"

class UC20_internet {
	public:
		UC20_internet(int8_t ctxid = 1);
		void configContext(const char* apn, const char* user, const char* pass);
		bool activate();
		bool activated();
		bool deactivate();
		String getLocalIP();
		bool ping(const char* server);
		void pingcmd(const char* server);
		bool getNTP(const char* server, unsigned int port = -1, bool settime = true);
		String recvNTP(unsigned long t_out = 12000);
		
	protected:
		int8_t _contextid;		// range (1-16)
		
	private:
		
};

#endif

// +QIURC: "closed",x
// +QIURC: "pdpdeact",x
// +QIURC: "recv",x
