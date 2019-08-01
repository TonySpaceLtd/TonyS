#include "TonySpace_RS485.h"
#include "TonyS_X1.h"


RS485::RS485(uint8_t slot)
{
	switch(slot)
	{
		case  SLOT1 :
			select_mode = IO1;
			pin_RX = RX1;
			pin_TX = TX1;
			break;
		case  SLOT2 :
			select_mode = IO3;
			pin_RX = RX1;
			pin_TX = TX1;
			break;
		case  SLOT3 :
			select_mode = IO5;
			pin_RX = RX1;
			pin_TX = TX1;
			break;
		case  SLOT4 :
			select_mode = IO7;
			pin_RX = RX2;
			pin_TX = TX2;
			break;
		case  SLOT5 :
			select_mode = IO9;
			pin_RX = RX2;
			pin_TX = TX2;
			break;
		case  SLOT6 :
			select_mode = IO11;
			pin_RX = RX2;
			pin_TX = TX2;
			break;
		case  SLOT1_U :
			select_mode = IO1;
			pin_RX = RX1;
			pin_TX = TX1;
			break;
		case  SLOT2_U :
			select_mode = IO3;
			pin_RX = RX1;
			pin_TX = TX1;
			break;
		case  SLOT3_U :
			select_mode = IO5;
			pin_RX = RX1;
			pin_TX = TX1;
			break;
		case  SLOT4_U :
			select_mode = IO7;
			pin_RX = RX2;
			pin_TX = TX2;
			break;
		case  SLOT5_U :
			select_mode = IO9;
			pin_RX = RX2;
			pin_TX = TX2;
			break;
		case  SLOT6_U :
			select_mode = IO11;
			pin_RX = RX2;
			pin_TX = TX2;
			break;
	}	
	Tony.pinMode(select_mode, OUTPUT);
}

void RS485::begin(uint32_t baud_rate)
{
	Serial2.begin(baud_rate, SERIAL_8N1, pin_RX, pin_TX);
}


String RS485::read()
{
	Tony.digitalWrite(select_mode, LOW);
	String recv;
	while (Serial2.available()) 
	{
		recv = recv+char(Serial2.read());
	}
	return(recv);
}

void RS485::print(String word)
{
	Tony.digitalWrite(select_mode, HIGH);
	Serial2.print(word);
}