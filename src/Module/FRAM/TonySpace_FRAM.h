// V.TS001
#ifndef TonySpace_FRAM_h
#define TonySpace_FRAM_h
#include "Arduino.h"

#define addrFRAM_P1 0x50
#define addrFRAM_P2 0x51


class TONY_FRAM
{
	public:
		TONY_FRAM();
		bool write(uint16_t addrData , uint32_t dataFRAM);
		uint32_t read(uint16_t addrRead_data);
		bool clear();
		uint16_t size(uint8_t step);
	private:
};
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
extern TONY_FRAM TonyFRAM;
#endif
#endif





















