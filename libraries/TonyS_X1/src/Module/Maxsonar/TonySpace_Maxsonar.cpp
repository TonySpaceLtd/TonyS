#include "TonyS_X1.h"
#include "TonySpace_Maxsonar.h"

HardwareSerial ser_sonar(NULL);


Maxsonar::Maxsonar()
{
	
}
void Maxsonar::begin(uint8_t slot)
{
	ser_sonar = Tony.SerialBegin(slot,9600);
}
int Maxsonar::read(unsigned long tout)
{
	int len = ser_sonar.available();
	if(len>0)
	{
		uint8_t b[len+1];
		ser_sonar.readBytes(b,len);
	}

	//ser_sonar.flush();
	unsigned long t_pv = millis();
	do
	{
		if(ser_sonar.available())
		{
			String c = ser_sonar.readStringUntil('R');
			c = ser_sonar.readStringUntil('R');
			return  c.toInt();
		}
	}
	while((millis() - t_pv)<tout);
	return -1;	
}