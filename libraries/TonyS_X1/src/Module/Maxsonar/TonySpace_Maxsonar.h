
#ifndef TonySpace_Maxsonar_h
#define TonySpace_Maxsonar_h

#include "Arduino.h"
#include "TonyS_X1.h"


class Maxsonar
{
	public:
		Maxsonar();
		void begin(uint8_t slot);
		int read(unsigned long tout=2000);
	private:
		
};



#endif





















