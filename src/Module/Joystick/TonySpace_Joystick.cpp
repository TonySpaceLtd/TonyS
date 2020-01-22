#include "TonySpace_Joystick.h"
#include "TonyS_X1.h"


JOYSTICK::JOYSTICK()
{
}
bool JOYSTICK::begin(char addr)
{
	bool ret = io.begin(addr);
	if(ret)
	{
		led1(1);
		led1(1);
		buzz(0);
	}
	return ret;
}
uint8_t JOYSTICK::getCursor()
{
	uint8_t i = ~io.read();
	return ((i&0b00011111));
}
