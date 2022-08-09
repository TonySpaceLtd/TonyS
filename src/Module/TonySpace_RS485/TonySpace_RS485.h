#ifndef TonySpace_RS485_h
#define TonySpace_RS485_h

#include "Arduino.h"
#include "Stream.h"


class Tony_RS485 : public Stream
{
public:
    Tony_RS485();
	void setSlot(uint8_t slot);
	void begin(unsigned long baud,uint32_t config = SERIAL_8N1);
	//Stream Class 
		virtual int read() { return ser_485->read();};	
		virtual size_t write(uint8_t c){return ser_485->write(c);};
		virtual int available(){return ser_485->available();};
		virtual int peek(){return ser_485->peek();}; 
		virtual void flush(){ser_485->flush();};
		using Stream::write; // make the other overloads of write visible

private:
		HardwareSerial *ser_485;
		uint8_t pin_RX, 
			pin_TX,
			_uart_nr;
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
extern Tony_RS485 RS485;
#endif

#endif
















