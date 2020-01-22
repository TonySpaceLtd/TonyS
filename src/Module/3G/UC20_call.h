#ifndef UC20_CALL_H
#define UC20_CALL_H

#include <Arduino.h>
#include "UC20_lib.h"

#define IDLE   0
#define RING   3
#define BUSY   4

class UC20_call {
	public:
		bool call(String phone_no);
		bool answer();
		bool disconnect();
		String currentCallME();
		int8_t listen();
		int8_t checkCallStat();
		int8_t getStat();
		
	private:
		int8_t _state = IDLE;
};

#endif
