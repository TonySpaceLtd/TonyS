#ifndef UC20_SSL_H
#define UC20_SSL_H

#include <Arduino.h>
#include "UC20_lib.h"

class UC20_SSL {
	public:
		UC20_SSL(int8_t sslctxid = 1);
		bool sslversion(int8_t version);
		bool ciphersuite(unsigned int type);
		bool seclevel(int8_t level);
		// bool cacert(const char* path);
		// bool clientcert(const char* path);
		// bool clientkey(const char* path);
		
	protected:
		int8_t _sslctxid;		// range (0-5)
		
	private:
		
};

#endif
