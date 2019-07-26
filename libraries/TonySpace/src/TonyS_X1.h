
#ifndef TonyS_X1_h
#define TonyS_X1_h

#include "TonySpace_MAX11301.h"
#include "TonySpace_Pins.h"
#include "TonySpace_IO.h"
#include "TonySpace_Plugin.h"



class TonyS_X1
{
public:
	TonyS_X1();
	void begin();
	void pinMode(uint8_t pin, uint8_t type);
	void digitalWrite(uint8_t pin, bool value);
	bool digitalRead(uint8_t pin);
	uint16_t analogRead(uint8_t pin);
	void analogWrite(uint8_t pin, float Output); // Output 0-10V
	void onPower(); // On power on board
	void offPower(); // Off power on board
	void checkIC(); // Check working IC on board
private:
	
};
extern TonyS_X1 Tony;
#endif





















