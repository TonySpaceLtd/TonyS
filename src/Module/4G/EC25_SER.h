#ifndef ECXX_SER_H
#define ECXX_SER_H

#include "Arduino.h"
#include <Stream.h>
#include "TonyS_X1.h"

extern HardwareSerial ser_LTE;

typedef struct EC_RES
{
	int id;
	String data;
	String temp;
}EC_Resp;

class EC25_SER : public Stream
{
	public:
		EC25_SER();
		void begin(uint8_t slot,unsigned long baud=115200);
		bool waitModuleReady();
		EC_Resp waitString(String data[],uint8_t len,long tout = 3000);
		EC_Resp waitString(String data,long tout=3000);
		void sendAT(String data,bool end=true);
		void sendAT(String data,String data2,bool end=true);
		String removeChar(String str,char cut);
		
	//Stream Class 
		virtual int read() { return ser_LTE.read();
		};	
		virtual size_t write(uint8_t c){return  ser_LTE.write(c);
		};
		virtual int available(){return ser_LTE.available();
		};
		virtual int peek(){return ser_LTE.peek();
		}; 
		virtual void flush()
		{ser_LTE.flush();
		};
		using Stream::write; // make the other overloads of write visible
};




#endif