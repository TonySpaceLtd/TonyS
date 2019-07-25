
#ifndef TonySpace_IO_h
#define TonySpace_IO_h

#include "arduino.h"

class TonySpace_IO
{
public:
	TonySpace_IO();
	void Real_pinMode(uint8_t pin, uint8_t type);
	void Real_digitalWrite(uint8_t pin, bool value);
	bool Real_digitalRead(uint8_t pin);
	uint16_t Real_analogRead(uint8_t pin);
private:
	
};
#endif





















