#ifndef UC20_HTTP_H
#define UC20_HTTP_H

#include <Arduino.h>
#include "UC20_lib.h"
#include "UC20_file.h"
#include "UC20_internet.h"
#include "UC20_SSL.h"

#define CUSTOM_HEADER      1
#define AUTOMATIC_HEADER   0
#define HTTPREAD_FILENAME  "RAM:temph2tp"

class UC20_HTTP : public UC20_internet, public UC20_SSL {
	public:
		UC20_HTTP(int8_t sslctxid = 1, int8_t ctxid = 1);
		void initConfig();
		void responseHeader(bool enable);
		void requestHeader(bool enable);	// used in GET and POST methods
		bool url(const char* server);
		bool get(int len = -1, int8_t mode = AUTOMATIC_HEADER);
		bool post(int len, int8_t mode = AUTOMATIC_HEADER);
		int wait_responseCode();
		bool save(String fname);
		long available();
		int read();
		String readString();
		
	private:
		UC20_file httpfile;
		uint8_t recentAct = 0;			// 0 : init value or idle
										// 1 : just sent GET
										// 2 : just sent POST
		bool saved = false;
		bool saveable = false;
		long _fsize = 0;
		long _fpos = 0;
};

#endif
