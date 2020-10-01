// V.TS001
#ifndef TonySpace_FRAM_h
#define TonySpace_FRAM_h

#include "Arduino.h"

#define UNKNOWN_MODEL      0x00
#define PRO_MODEL          0x01
#define BASIC_MODEL        0x02


class T_FRAM
{
	public:
		T_FRAM(uint8_t slot);
		uint16_t readValue();	
	private:
		uint8_t pin;
};
#endif





















