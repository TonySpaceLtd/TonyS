
#ifndef TonySpace_RS485_h
#define TonySpace_RS485_h

#include "Arduino.h"



class RS485
{
	public:
		RS485(uint8_t slot);
		void begin(uint32_t baud_rate);
		String read();
		void print(String word);	
	private:
		uint8_t	select_mode;
		uint8_t	pin_RX;
		uint8_t	pin_TX;
};
#endif





















