#ifndef UC20_LIB_H
#define UC20_LIB_H

#include <Arduino.h>
#include "UCXX_SER.h"

#define POWERPIN   0

class UC20 : public UCXX_SER {
	public:
		bool powerOn(int pin = POWERPIN);
		bool powerOff(int pin = POWERPIN);
		void initConfig();
		bool simReady();
		bool getNetworkStatus();
		int8_t getSignal();
		String getIMEI();
		String getIMSI();
		String getICCID();
		String getOperator();
		String getFirmwareVersion();
		String moduleTime();
		
	private:
		
};
extern UC20 gsm;

#endif

// call
// SMS
// SMTP
