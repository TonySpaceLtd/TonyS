#include "TonySpace_IO.h"

TonySpace_IO::TonySpace_IO()
{
	
}


void TonySpace_IO::Real_pinMode(uint8_t pin, uint8_t type)
{
	pinMode(pin, type);
}

void TonySpace_IO::Real_digitalWrite(uint8_t pin, bool value)
{
	digitalWrite(pin, value);
}

bool TonySpace_IO::Real_digitalRead(uint8_t pin)
{
	bool value = 0;
	value = digitalRead(pin);
	return(value);
}

uint16_t TonySpace_IO::Real_analogRead(uint8_t pin)
{
	float dataADC = 0;
	dataADC = analogRead(pin);
	return(dataADC);
}
