#include "TonySpace_4_20mA.h"
#include "TonyS_X1.h"

MAX11301 MAX11301_1;


T4_20mA::T4_20mA(uint8_t slot)
{
	switch(slot)
	{
		case  SLOT1 :
			pin = AIO0;
			break;
		case  SLOT2 :
			pin = AIO2;
			break;
		case  SLOT3 :
			pin = AIO4;
			break;
		case  SLOT4 :
			pin = AIO6;
			break;
		case  SLOT5 :
			pin = AIO8;
			break;
		case  SLOT6 :
			pin = AIO10;
			break;
		case  SLOT1_U :
			pin = AIO1;
			break;
		case  SLOT2_U :
			pin = AIO3;
			break;
		case  SLOT3_U :
			pin = AIO5;
			break;
		case  SLOT4_U :
			pin = AIO7;
			break;
		case  SLOT5_U :
			pin = AIO9;
			break;
		case  SLOT6_U :
			pin = AIO11;
			break;
	}	
}


uint16_t T4_20mA::readValue()
{
	uint16_t dataADC = 0;
	MAX11301_1.Basic_Config_Port(pin, ADCtype1);  
	dataADC = MAX11301_1.readADC(pin); 
	return(dataADC);
}


