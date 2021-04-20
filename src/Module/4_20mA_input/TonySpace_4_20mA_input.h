// V.TS001
#ifndef TonySpace_4_20mA_h
#define TonySpace_4_20mA_h

#include "Arduino.h"

#define UNKNOWN_MODEL      0x00
#define PRO_MODEL          0x01
#define BASIC_MODEL        0x02


class T4_20mA
{
	public:
		T4_20mA(uint8_t slot);
		uint16_t readValue();	
	private:
		uint8_t pin;
};
#endif





















