// V.TS001
#include "TonySpace_FRAM.h"
#include "TonyS_X1.h"

TonyS_X1 Tony_FRAM;

extern byte board_model;

T_FRAM::T_FRAM(uint8_t slot)
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

uint16_t T_FRAM::readValue()
{
	uint16_t dataADC = 0;
	dataADC = Tony_FRAM.analogRead(pin);
	return(dataADC);
}


