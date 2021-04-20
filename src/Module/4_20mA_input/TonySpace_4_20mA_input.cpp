// V.TS001
#include "TonySpace_4_20mA_input.h"
#include "TonyS_X1.h"

TonyS_X1 Tony_4_20;

extern byte board_model;

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
	dataADC = Tony_4_20.analogRead(pin);
	return(dataADC);
}


