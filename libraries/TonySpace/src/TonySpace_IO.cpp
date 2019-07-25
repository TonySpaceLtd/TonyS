#include "TonySpace_IO.h"

TonySpace_IO::TonySpace_IO()
{
	
}


void TonySpace_IO::Real_pinMode(uint8_t pin, uint8_t type)
{
	uint8_t setPin = pin;
	setPin = setPin-100;
	pinMode(setPin, type);
}

void TonySpace_IO::Real_digitalWrite(uint8_t pin, bool value)
{
	uint8_t setPin = pin;
	setPin = setPin-100;
	digitalWrite(setPin, value);
}

bool TonySpace_IO::Real_digitalRead(uint8_t pin)
{
	bool value = 0;
	uint8_t setPin = pin;
	setPin = setPin-100;
	value = digitalRead(setPin);
	return(value);
}

uint16_t TonySpace_IO::Real_analogRead(uint8_t pin)
{
	float dataADC = 0;
	uint8_t setPin = pin;
	setPin = setPin-100;
	dataADC = analogRead(setPin);
	return(dataADC);
}
