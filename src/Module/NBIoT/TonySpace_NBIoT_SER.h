
#ifndef TonySpace_NBIoT_SER_h
#define TonySpace_NBIoT_SER_h

#include "Arduino.h"
#include <Stream.h>
#include "TonyS_X1.h"

extern HardwareSerial ser_nbiot;

typedef struct BC_RES
{
	int id;
	String data;
	String temp;
}atResp;

class NBIoT_SER : public Stream
{
	public:
		NBIoT_SER();
		void begin(uint8_t slot,unsigned long baud);
		atResp waitString(String data[],uint8_t len,long tout = 3000);
		atResp waitString(String data,long tout=3000);
		void sendAT(String data,bool end=true);
		
		
		
		
		//Stream Class 
		virtual int read() { return ser_nbiot.read();};	
		virtual size_t write(uint8_t c){return ser_nbiot.write(c);};
		virtual int available(){return ser_nbiot.available();};
		virtual int peek(){return ser_nbiot.peek();}; 
		virtual void flush(){ser_nbiot.flush();};
		using Stream::write; // make the other overloads of write visible
	
	private:
		
		
};
#endif





















