#ifndef UCXX_SER_H
#define UCXX_SER_H

#include <Arduino.h>
#include <Stream.h>
#include "TonyS_X1.h"

#define DEFAULT_DEBUG_SER   Serial
#define BUFFER_SIZE         200

class UCXX_SER {
	public:
		Stream* debug = &DEFAULT_DEBUG_SER;		// debugging serial port
		
		void initSerial(Stream &serial);
		void initial(uint8_t slot,unsigned long baud=115200);
		void setDebugSerial(Stream &serial);
		template <typename t> size_t print  (t data)             { return _ucSerial->print(data);           }
		template <typename t> size_t print  (t data, int format) { return _ucSerial->print(data, format);   }
		template <typename t> size_t println(t data)             { return _ucSerial->println(data);         }
		template <typename t> size_t println(t data, int format) { return _ucSerial->println(data, format); }
		template <typename t> size_t write  (t data)             { return _ucSerial->write(data);           }
		template <typename t> size_t write  (t* data, int len)   { return _ucSerial->write(data, len);      }
		size_t println();
		int available();
		int read();
		int peek();
		String readStringUntil(const char terminate);
		String waitStr(String str, unsigned long t_out = 3000, String strNot = "", bool clrLF = true);
		
		long debugReadUntil(const char* terminate);
		void readKeepUntil(const char* terminate, int userid = 0);
		void readKeepFor(unsigned int times, int userid = 0);
		bool getKeepUntilDone();
		int  available_buf();
		int  read_buf();
		String readString_buf();
		void clear_buf();
		int  getUser_buf();
		
		void printUnicode(String str);
		void printlnUnicode(String str);
		void printlnUnicode();
		uint8_t strToHex(String str);
		String ucodeToUtf8(String ucode, int len_byte = 2);
		String utf8ToUcode(String utf8);
		uint8_t powerPin;
		
	protected:
		Stream* _ucSerial;						// communicate with UCXX
		char charT[BUFFER_SIZE+1];
		char* buf = charT;
		bool readUntilEnd = true;
	
	private:
		uint8_t buf_cur = 0;
		uint8_t buf_len = 0;
		int buf_owner_id = -1;					// -1        : init value (no owner)
												// 0         : general user
												// 100-111   : TCPIP features (connect id)
												// 1000-5000 : FILE features (file handle)
};

#endif
