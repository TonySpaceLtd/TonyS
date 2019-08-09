
#ifndef TonySpace_4_20mA_h
#define TonySpace_4_20mA_h

#include "Arduino.h"



class T4_20mA
{
	public:
		T4_20mA(uint8_t slot);
		void selectslot(uint8_t slot);
		uint16_t readValue();	
	private:
		uint8_t pin;
};
#endif





















