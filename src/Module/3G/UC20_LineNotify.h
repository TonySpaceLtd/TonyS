#ifndef UC20_LINE_NOTIFY_H
#define UC20_LINE_NOTIFY_H

#include <Arduino.h>
#include "UC20_lib.h"
#include "UC20_HTTP.h"

#define LINEHOST   "https://notify-api.line.me/api/notify"

class UC20_LineNotify {
	public:
		void begin(const char* token);
		void sticker(long StkPkgID, long StkID);
		void imageurl(String url);
		void silentNotify(bool enable);
		bool submit(String msg);
		void clear();
		long available();
		int read();
		
	private:
		UC20_HTTP _client;
		String _token  = "";
		String _data[5];
		uint8_t _dataFlag = 0b00000000;	// bit 0 (LSB) ; text
										// bit 1-2     ; sticker
										// bit 3       ; image
										// bit 4       ; notification disabled
										// bit 5-7     ; reserved
};

#endif
